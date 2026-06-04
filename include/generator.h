#include <iostream>
// Para generar números pseudoaleatorios
// Para leer datos entrópicos de /dev/random (ó /dev/urandom)
#include <fstream>
#include <cstdint>

class Generator{
private:
    uint64_t _seed = 0;

    /**
     * @author Alejandro Anglada Álvarez
     * @date 04-06-2026
     * 
     * @brief Sets this object's seed to the parameter.
     * 
     * @param s New seed.
     * 
     * @pre The uint64_t must represent a safe, pseudo-randomly generated seed.
     * @post This object's _seed is now set to s.
     * 
     * @return true if the _seed value was changed correctly, false otherwise.
     */
    bool setSeed(uint64_t s);

    /**
     * @author Alejandro Anglada Álvarez
     * @date 04-06-2026
     * 
     * @brief Adds a random amount given by UNIX-based kernels to the seed.
     * 
     * @return An unsigned 64-bit integer, that represents a random value.
     */
    uint64_t readKernelEntropy();

public:
    /**
     * @author Alejandro Anglada Álvarez
     * @date 04-06-2026
     * 
     * @brief Generates a valid object of Generator type.
     * 
     * @post The newly-created Generator object has a pseudo-randomly generated
     * _seed, associated with the current process' clock.
     */
    Generator();

    /**
     * @author Alejandro Anglada
     * @date 04-06-2026
     * 
     * @brief Generates a valid object of Generator type, given a seed.
     * 
     * @pre The seed must be a valid instance of a uint64_t data.
     * @post The newly-created Generator object has a _seed set to seed parameter.
     */
    Generator(uint64_t seed);

    /** 
     * @author Alejandro Anglada
     * @date 04-06-2026
     * 
     * @brief Returns the current seed value.
     * 
     * @return The current seed value.
    */
    uint64_t getSeed();

    /**
     * @author Alejandro Anglada Álvarez
     * @date 04-06-2026
     * 
     * @brief Static method, does bit-wise operations and sums to a value so it
     * is harder to decypher. It is a murmur avalanche algorithm, which means:
     * - Deterministic, but a small change in the input (i.e. a bit changing) results
     * in drastically different outputs.
     * - Murmur, which multiplies and rotates the value in different ways so it is not
     * the same input value. ((MUltiplicate & Rotate)^2)
     * 
     * @param x Value that gets manipulated.
     * 
     * @pre x must be an initialized, correctly-functioning value.
     * @return x after getting manipulated ()
     */
    static uint64_t mix64(uint64_t x);
};