#include <iostream>
#include <limits>
#include "include/passgen.h"
#include "include/file_manager.h"

static std::string generarContrasena() {
    int caracs = 0;
    char nums, symb;
    bool bNums = false, bSymb = false;

    std::cout << "Interfaz de generacion de contrasenas. ¿Cuantos caracteres desea?\n";
    std::cin >> caracs;
    while (caracs <= 0) {
        std::cout << "Valor invalido. Introduzca un numero positivo.\n";
        std::cin >> caracs;
    }

    std::cout << "¿Desea que contenga numeros? (Y/N)\n";
    do {
        std::cin >> nums;
        if (std::tolower(nums) == 'y' || std::tolower(nums) == 's')
            bNums = true;
        else if (std::tolower(nums) == 'n')
            bNums = false;
        else
            std::cout << "Valor invalido. Introduzca 'Y' o 'N'.\n";
    } while (nums != 'Y' && nums != 'y' && nums != 'S' && nums != 's' && nums != 'N' && nums != 'n');

    std::cout << "¿Desea que contenga simbolos? (Y/N)\n";
    do {
        std::cin >> symb;
        if (std::tolower(symb) == 'y' || std::tolower(symb) == 's')
            bSymb = true;
        else if (std::tolower(symb) == 'n')
            bSymb = false;
        else
            std::cout << "Valor invalido. Introduzca 'Y' o 'N'.\n";
    } while (symb != 'Y' && symb != 'y' && symb != 'S' && symb != 's' && symb != 'N' && symb != 'n');

    PassGen nuevo(bNums, bSymb);
    std::string contra = nuevo.generate(caracs);
    std::cout << "Su contrasena es: " << contra << '\n';
    return contra;
}

static void guardarContrasena(FileManager &fm) {
    std::string pwd = generarContrasena();
    std::string nombre;

    std::cout << "Interfaz de guardado de contrasenas. ¿Con que nombre desea guardar la contrasena?\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, nombre);

    std::cout << "Guardando...\n";
    if (!fm.savePassword(nombre, pwd)) {
        std::cout << "Error al guardar la contrasena.\n";
    } else {
        std::cout << "Exito al guardar la contrasena.\n";
    }
}

static void runInterface(FileManager &fm) {
    int opcion = -1;

    while (opcion != 3) {
        std::cout << "\nInterfaz de PassGen. Escoja una opcion:\n";
        std::cout << "1. Generar contrasena (No sera guardada).\n";
        std::cout << "2. Guardar contrasena en \"passwords.json\".\n";
        std::cout << "3. Salir.\n";

        std::cin >> opcion;
        if (opcion == 1)
            generarContrasena();
        else if (opcion == 2)
            guardarContrasena(fm);
        else if (opcion != 3)
            std::cout << "Opcion invalida. Escoja otra.\n";
    }

    std::cout << "Saliendo...\n";
}

int main() {
    FileManager fm("passwords.json");
    runInterface(fm);
    return 0;
}
