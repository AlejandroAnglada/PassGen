#include "../include/passgen.h"

PassGen::PassGen(bool num, bool symb) : _key(), _num(num), _symbols(symb) {}

bool PassGen::getNum() const {
    return this->_num;
}

bool PassGen::getSymbols() const {
    return this->_symbols;
}

Key& PassGen::getKey() {
    return this->_key;
}

std::string PassGen::generate(size_t len){
    std::string min = "qwertyuiopasdfghjklzxcvbnm";
    std::string may = "QWERTYUIOPASDFGHJKLZXCVBNM";
    std::string num = "1234567890";
    std::string symb = "¡!¿?;:,.*^-_'@#$&()[]{}=|/<>";

    std::string all = min + may;
    if(this->getNum())
        all += num;
    if(this->getSymbols())
        all += symb;

    std::string pwd;

    // Luego vamos a mezclar, así que no pasa nada, así garantizamos mínimo uno de cada
    // uno de los seleccionados!
    pwd += min[this->getKey().range(0, min.length() - 1)];
    pwd += may[this->getKey().range(0, may.length() - 1)];
    if(this->getNum())
        pwd += num[this->getKey().range(0, num.length() - 1)];
    if(this->getSymbols())
        pwd += symb[this->getKey().range(0, symb.length() - 1)];
        
    // Rellenamos...
    while (pwd.length() < len) {
        pwd += all[this->getKey().range(0, all.length() - 1)];
    }

    // Ahora usamos Fisher-Yates para barajar (FY garantiza equiprobabilidad en resultados)
    for (int i = pwd.length() - 1; i > 0; --i) {
        int j = this->getKey().range(0, i); // Elige un índice entre 0 e i
        // Intercambia los caracteres
        char temp = pwd[i];
        pwd[i] = pwd[j];
        pwd[j] = temp;
    }

    return pwd;
}