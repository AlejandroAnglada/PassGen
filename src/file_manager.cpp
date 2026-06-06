#include "../include/file_manager.h"

void FileManager::initialize(std::string path){
    // por ahora no tiene funcionalidad, pero podría reusarse cuando haya GUI
    // para validar formato JSON o lo que sea :P
}

bool FileManager::open(std::string path){
    this->_path = path;
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
    //    1.2. Comprobar formato (método global)
    // 2. En dicho string hacemos la lógica pertinente
    //    2.1. Si se encuentra la key, se actualiza la pwd
    //    2.2. Si no, se añade al final antes del }
    // 3. Se sobrescribe el fichero
    // Usando bibliotecas de 3os seguro que se puede hacer más fácil, pero lo
    // suyo es que dependa de código ajeno lo menos posible :)
    _json.seekg(0); // Pone el cursor a 0
    std::string contenido = std::string(
        std::istreambuf_iterator<char>(_json), // Añade el contenido desde el iterador del principio...
        std::istreambuf_iterator<char>()       // ...hasta EOF (iterador por defecto).
    );
    // Si no tiene buen formato JSON a tomar viento
    if(!FileManager::isValidJson(contenido)) return false;

    // String a buscar
    std::string clave = '\"' + key + '\"';
    size_t posClave = contenido.find(clave);
    bool encontrado = false;
    // Mientras que no se llegue al final de la línea
    while (posClave != std::string::npos) {
        // Verificar que sea una clave, y no el valor (MUY poco probable pero puede ser)
        size_t after = posClave + clave.length();
        while (after < contenido.length() && (contenido[after] == ' ' || contenido[after] == '\t'))
            after++;
        if (after < contenido.length() && contenido[after] == ':') {
            encontrado = true;
            break;
        }
        // Si no era la clave, seguir buscando
        posClave = contenido.find(clave, posClave + 1);
    }

    // Ahora puede pasar que bien se haya encontrado o bien no
    if(encontrado){
        size_t dosPuntos = contenido.find(':', posClave);
        size_t pwdStart = contenido.find('"', dosPuntos + 1);
        size_t pwdEnd   = contenido.find('"', pwdStart + 1);

        if (pwdStart == std::string::npos || pwdEnd == std::string::npos)
            return false;

        contenido.replace(pwdStart + 1, pwdEnd - pwdStart - 1, pwd);
    } else {
        size_t ultimoCorch = contenido.rfind('}');
        if (ultimoCorch == std::string::npos) return false;
        // Si el JSON solo tiene "{}", no poner coma al principio (StackOverflow)
        bool empty = (contenido.find_first_not_of(" \t\n\r{}") == std::string::npos);
        std::string nuevaEntrada;
        if (empty)
            nuevaEntrada = "\"" + key + "\": \"" + pwd + "\"";
        else
            nuevaEntrada = ", \"" + key + "\": \"" + pwd + "\"";

        contenido.insert(ultimoCorch, nuevaEntrada);

    }

    // Sobrescribir el archivo (truncar y reescribir)
    _json.close();
    _json.open(_path, std::fstream::out | std::fstream::trunc);
    _json << contenido;
    _json.close();
    _json.open(_path, std::fstream::in | std::fstream::out);
    _isOpen = _json.is_open();

    return _isOpen;
}




bool FileManager::isValidJson(std::string jsonContents){
    int corchetes = 0;
    bool enString = false;
    for(auto it = jsonContents.begin(); it != jsonContents.end(); it++){
        if(*it == '\\'){
            it++;
            continue; // Carácter escapado
        }
        if(*it == '"'){
            enString = !enString;
            continue; // Empezamos (o terminamos) un string, estos corchetes no deben tenerse en cuenta.
        }
        if(!enString){
            if(*it == '{') corchetes++;
            if(*it == '}') corchetes--;
        }
    }
    return corchetes == 0;
}