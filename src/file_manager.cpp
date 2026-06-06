#include "../include/file_manager.h"
#include <stack>

void FileManager::initialize(std::string path){
    // por ahora no tiene funcionalidad, pero podría reusarse cuando haya GUI
    // para validar formato JSON o lo que sea :P
}

bool FileManager::open(std::string path){
    this->_json.open(path, std::fstream::out | std::fstream::in); // Se abre en rw para no truncar!
    // Comprobamos si ha sido abierto. Si sí, existe y se cierra. Si no, se crea.
    if(this->_json.fail()){
        this->_json.clear();                                          // Reset de flags
        this->_json.open(path,std::fstream::out);                     // Se crea en rs
        this->_json.close();                                          // Se cierra (tiene que ser de entrada y salida)
        this->_json.open(path, std::fstream::in | std::fstream::out); // Se vuelve a abrir, ahora en rw
        this->_json << "{}";                                          // Fichero JSON sin datos
    }
    return (this->_isOpen = this->_json.is_open());
}

FileManager::FileManager(std::string name){
    if(!this->open(name)){
        throw std::runtime_error("Could not open file located in: " + name);
    }
}

bool FileManager::close(){
    if(this->isOpen()){
        this->_json.close();
        this->_isOpen = false;
    }
    return !this->_json.fail();
}

bool FileManager::isOpen() const{
    return this->_isOpen;
}

FileManager::~FileManager(){
    this->close();
}

bool FileManager::savePassword(std::string key, std::string pwd){
    // El flujo es el siguiente:
    // 1. Obtener todo el texto (guardamos todo el fichero a un String)
    // 2. En dicho string hacemos la lógica pertinente
    //    2.1. Si se encuentra la key, se actualiza la pwd
    //    2.2. Si no, se añade al final antes del }
    // 3. Se sobrescribe el fichero
    // Usando bibliotecas de 3os seguro que se puede hacer más fácil, pero lo
    // suyo es que dependa de código ajeno lo menos posible :)
    std::string contenido = std::string(
        std::istreambuf_iterator<char>(_json), // Añade el contenido desde el iterador del principio...
        std::istreambuf_iterator<char>()       // ...hasta EOF (iterador por defecto).
    );

    if(!FileManager::isValidJson(contenido)) return false;

    // TODO implementar isValidJson y terminar savePassword
}