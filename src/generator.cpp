#include "../include/generator.h"

// Private

bool Generator::setSeed(uint64_t s){
    uint64_t currSeed = this->_seed;
    this->_seed = s;
    return this->_seed != currSeed;
}

uint64_t Generator::readKernelEntropy(){
    // El dispositivo /dev/urandom tiene datos entrópicos en los sistemas UNIX.
    // Lee cosas como interferencias electromagnéticas, temperatura, posición
    // del ratón, etc para generar "ruido" extremadamente difícil de predecir.
    std::ifstream entropyU("/dev/urandom", std::ios::binary);
    if (!entropyU)
        throw std::runtime_error("Error opening /dev/urandom.\nTip: Is this an UNIX device?");

    uint64_t a, b;
    entropyU.read(reinterpret_cast<char*>(&a), sizeof(a));
    entropyU.read(reinterpret_cast<char*>(&b), sizeof(b));

    if (!entropyU)
        throw std::runtime_error("Error reading from /dev/urandom.");

    // Murmur con avalancha, operación AND para mayor confusión
    return Generator::mix64(a) ^ Generator::mix64(b);
}

// Public

Generator::Generator(){
    // Usa de semilla la entropía actual del sistema
    uint64_t currTime = this->readKernelEntropy();
    if(!this->setSeed(currTime))
        std::cerr << "WARNING! Seed not initialized correctly!";
}

Generator::Generator(uint64_t seed){
    // Asociamos la semilla!
    if(!this->setSeed(seed))
        std::cerr << "WARNING! Seed not initialized correctly!";
}

uint64_t Generator::getSeed(){
    return this->_seed;
}

uint64_t Generator::mix64(uint64_t x){
    // Los valores mágicos son del estándar MurmurHash3, no son inventados.
    x ^= x >> 30;
    x *= 0xbf58476d1ce4e5b9ULL;
    x ^= x >> 27;
    x *= 0x94d049bb133111ebULL;
    x ^= x >> 31;
    return x;
}