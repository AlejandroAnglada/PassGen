#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <cstdlib>
#include "../include/file_manager.h"

#define TEST_FILE "test_passwords.json"
#define ANSI_GREEN  "\033[32m"
#define ANSI_RED    "\033[31m"
#define ANSI_RESET  "\033[0m"

static int testsPassed = 0;
static int testsFailed = 0;

void check(bool condition, const char* msg) {
    if (condition) {
        std::cout << ANSI_GREEN << "  [PASS] " << ANSI_RESET << msg << "\n";
        testsPassed++;
    } else {
        std::cout << ANSI_RED << "  [FAIL] " << ANSI_RESET << msg << "\n";
        testsFailed++;
    }
}

std::string readFile() {
    std::ifstream f(TEST_FILE);
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

bool contentContains(const std::string& content, const std::string& key, const std::string& pwd) {
    std::string expected = "\"" + key + "\": \"" + pwd + "\"";
    return content.find(expected) != std::string::npos;
}

bool contentContainsRaw(const std::string& content, const std::string& str) {
    return content.find(str) != std::string::npos;
}

int countOccurrences(const std::string& content, const std::string& key) {
    std::string search = "\"" + key + "\"";
    int count = 0;
    size_t pos = 0;
    while ((pos = content.find(search, pos)) != std::string::npos) {
        // Check it's a key (followed by ':')
        size_t after = pos + search.length();
        while (after < content.length() && (content[after] == ' ' || content[after] == '\t'))
            after++;
        if (after < content.length() && content[after] == ':')
            count++;
        pos++;
    }
    return count;
}

void cleanUp() {
    std::remove(TEST_FILE);
}

// ──────────────────────────────────────────────────

void testConstructor() {
    std::cout << "\033[1m\n--- Constructor ---\033[0m\n";
    {
        FileManager fm(TEST_FILE);
        check(fm.isOpen(), "Archivo abierto tras construir");
    }
    // El destructor se llama aquí, cierra el archivo
    check(readFile() == "{}", "Archivo creado con JSON vacio");
    cleanUp();
}

void testSaveSingle() {
    std::cout << "\033[1m\n--- Guardar una entrada ---\033[0m\n";
    {
        FileManager fm(TEST_FILE);
        check(fm.savePassword("github", "pass123"), "savePassword devuelve true");
    }
    std::string content = readFile();
    check(contentContains(content, "github", "pass123"), "Contenido contiene clave:password");
    check(countOccurrences(content, "github") == 1, "La clave aparece exactamente 1 vez");
    check(FileManager::isValidJson(content), "El JSON generado es valido");
    cleanUp();
}

void testSaveMultiple() {
    std::cout << "\033[1m\n--- Guardar multiples entradas ---\033[0m\n";
    {
        FileManager fm(TEST_FILE);
        check(fm.savePassword("github", "pass123"), "savePassword github");
        check(fm.savePassword("gmail",  "mailpass"), "savePassword gmail");
        check(fm.savePassword("reddit", "res4life"), "savePassword reddit");
    }
    std::string content = readFile();
    check(contentContains(content, "github", "pass123"), "github presente");
    check(contentContains(content, "gmail",  "mailpass"), "gmail presente");
    check(contentContains(content, "reddit", "res4life"), "reddit presente");
    check(countOccurrences(content, "github") == 1, "github aparece 1 vez");
    check(countOccurrences(content, "gmail")  == 1, "gmail aparece 1 vez");
    check(countOccurrences(content, "reddit") == 1, "reddit aparece 1 vez");
    check(FileManager::isValidJson(content), "JSON valido");
    cleanUp();
}

void testUpdateExisting() {
    std::cout << "\033[1m\n--- Actualizar entrada existente ---\033[0m\n";
    {
        FileManager fm(TEST_FILE);
        fm.savePassword("github", "pass123");
        fm.savePassword("gmail",  "mailpass");
        check(fm.savePassword("github", "nuevaPass"), "Actualizar github devuelve true");
    }
    std::string content = readFile();
    check(contentContains(content, "github", "nuevaPass"), "github tiene la nueva password");
    check(!contentContainsRaw(content, "pass123"), "La password antigua ya no aparece");
    check(contentContains(content, "gmail", "mailpass"), "gmail sigue intacto");
    check(countOccurrences(content, "github") == 1, "github sigue apareciendo 1 vez");
    check(countOccurrences(content, "gmail")  == 1, "gmail sigue apareciendo 1 vez");
    check(FileManager::isValidJson(content), "JSON sigue siendo valido tras actualizar");
    cleanUp();
}

void testUpdateMultipleTimes() {
    std::cout << "\033[1m\n--- Actualizar la misma clave varias veces ---\033[0m\n";
    {
        FileManager fm(TEST_FILE);
        fm.savePassword("github", "v1");
        fm.savePassword("github", "v2");
        fm.savePassword("github", "v3_final");
    }
    std::string content = readFile();
    check(contentContains(content, "github", "v3_final"), "Version final correcta");
    check(!contentContainsRaw(content, "v1"), "v1 no aparece");
    check(!contentContainsRaw(content, "v2"), "v2 no aparece");
    check(countOccurrences(content, "github") == 1, "Una sola clave github");
    check(FileManager::isValidJson(content), "JSON valido");
    cleanUp();
}

void testPasswordWithSpecialChars() {
    std::cout << "\033[1m\n--- Password con caracteres especiales ---\033[0m\n";
    {
        FileManager fm(TEST_FILE);
        check(fm.savePassword("site1", "abc{}[]:,"), "Password con llaves, corchetes, coma y dos puntos");
        check(fm.savePassword("site2", "pwd!?@#$&*"), "Password con simbolos del generador");
    }
    std::string content = readFile();
    check(contentContains(content, "site1", "abc{}[]:,"), "Llaves, corchetes, coma y dos puntos en JSON");
    check(contentContains(content, "site2", "pwd!?@#$&*"), "Simbolos especiales en JSON");
    check(FileManager::isValidJson(content), "JSON sigue siendo valido");
    cleanUp();
}

void testPreserveOnReopen() {
    std::cout << "\033[1m\n--- Persistencia al reabrir el archivo ---\033[0m\n";
    {
        FileManager fm(TEST_FILE);
        fm.savePassword("github", "pass123");
        fm.savePassword("gmail",  "mailpass");
    }
    // Abrimos el mismo archivo de nuevo
    {
        FileManager fm(TEST_FILE);
        check(fm.isOpen(), "Se reabre el archivo existente");
        check(fm.savePassword("reddit", "res4life"), "Se anade otra clave");
    }
    std::string content = readFile();
    check(contentContains(content, "github", "pass123"), "Datos previos preservados");
    check(contentContains(content, "gmail",  "mailpass"), "Datos previos preservados");
    check(contentContains(content, "reddit", "res4life"), "Nuevos datos anadidos");
    check(FileManager::isValidJson(content), "JSON valido tras reabrir");
    cleanUp();
}

void testCloseAndIsOpen() {
    std::cout << "\033[1m\n--- close e isOpen ---\033[0m\n";
    FileManager fm(TEST_FILE);
    check(fm.isOpen(), "isOpen true tras construir");
    check(fm.close(), "close devuelve true");
    check(!fm.isOpen(), "isOpen false tras close");
    check(!fm.savePassword("x", "y"), "savePassword falla si no esta abierto");
    cleanUp();
}

void testIsValidJson() {
    std::cout << "\033[1m\n--- isValidJson (casos borde) ---\033[0m\n";
    check(FileManager::isValidJson("{}"), "Vacio");
    check(FileManager::isValidJson("{\"a\": \"b\"}"), "Simple");
    check(FileManager::isValidJson("{\"a\": \"{\", \"b\": \"}\"}"), "Llaves dentro de strings");
    check(!FileManager::isValidJson("{"), "Solo abierto");
    check(!FileManager::isValidJson("}"), "Solo cerrado");
    check(FileManager::isValidJson("{a}"), "isValidJson cuenta solo llaves, no valida tokens");
    check(FileManager::isValidJson("{\"escaped\\\"quote\": \"val\"}"), "Comillas escapadas");
    check(FileManager::isValidJson("{\"nested\": {\"inner\": \"val\"}}"), "Objeto anidado");
    cleanUp();
}

int main() {
    std::cout << "\033[1m=== Tests FileManager ===\033[0m\n";

    testConstructor();
    testSaveSingle();
    testSaveMultiple();
    testUpdateExisting();
    testUpdateMultipleTimes();
    testPasswordWithSpecialChars();
    testPreserveOnReopen();
    testCloseAndIsOpen();
    testIsValidJson();

    std::cout << "\n\033[1m=== Resumen:\033[0m " << (testsPassed + testsFailed)
              << " tests, " << ANSI_GREEN << testsPassed << " pasados" << ANSI_RESET
              << ", " << ANSI_RED << testsFailed << " fallidos" << ANSI_RESET << " ===\n";

    return testsFailed > 0 ? 1 : 0;
}
