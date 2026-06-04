#include <iostream>
#include "../include/key.h"

// Private

bool Key::setSeed(uint64_t s){
    uint64_t currSeed = this->_seed;
    this->_seed = s;
    return this->_seed != currSeed;
}

uint64_t Key::readKernelEntropy(){
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
    return Key::mix64(a) ^ Key::mix64(b);
}

// Public

Key::Key(){
    // Usa de semilla la entropía actual del sistema
    uint64_t currTime = this->readKernelEntropy();
    if(!this->setSeed(currTime))
        std::cerr << "WARNING! Seed not initialized correctly!";
}

uint64_t Key::getSeed() const {
    return this->_seed;
}

uint64_t Key::mix64(uint64_t x){
    // Los valores mágicos son del estándar MurmurHash3, no son inventados.
    x ^= x >> 30;
    x *= 0xbf58476d1ce4e5b9ULL;
    x ^= x >> 27;
    x *= 0x94d049bb133111ebULL;
    x ^= x >> 31;
    return x;
}

uint64_t Key::next(){
    uint64_t key = this->_seed;
    key += 0x9e3779b97f4a7c15;
    key = (key ^ (key >> 30)) * 0xbf58476d1ce4e5b9;
    key = (key ^ (key >> 27)) * 0x94d049bb133111eb;
    this->_seed = key ^ (key >> 31);
    return this->_seed;
}

uint64_t Key::range(int min, int max){
    return min + (this->next() % (max - min + 1));
}