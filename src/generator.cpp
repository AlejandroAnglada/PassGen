#include "../include/generator.h"

// Private

bool Generator::setSeed(u_int32_t s){
    u_int32_t currSeed = this->_seed;
    this->_seed = s;
    return this->_seed != currSeed;
}

// Public

Generator::Generator(){
    // Usa de semilla el instante 0 del tiempo actual
    u_int32_t currTime = time(0);
    // Ahora, creamos nueva semilla aleatoria con dicho instante
    srand(currTime);
    if(!this->setSeed(rand()))
        exit(-1);
}

Generator::Generator(u_int32_t seed){
    // Asociamos la semilla!
    if(!this->setSeed(seed))
        exit(-1);
}

u_int32_t Generator::getSeed(){
    return this->_seed;
}