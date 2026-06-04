#include <iostream>
#include "include/passgen.h"

int main(){
    PassGen pg;

    std::string str = pg.generate(16);

    std::cout << str;
}