#ifndef KEY_H
#define KEY_H

// Para generar números pseudoaleatorios
// Para leer datos entrópicos de /dev/random (ó /dev/urandom)
#include <fstream>
#include <cstdint>

class Key{
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
     * @brief Generates a valid object of Key type.
     * 
     * @post The newly-created Key object has a pseudo-randomly generated
     * _seed, associated with the current process' clock.
     */
    Key();

    /** 
     * @author Alejandro Anglada
     * @date 04-06-2026
     * 
     * @brief Returns the current seed value.
     * 
     * @return The current seed value.
    */
    uint64_t getSeed() const;

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

    /**
     * @author Alejandro Anglada Álvarez
     * @date 04-06-2026
     * 
     * @brief Outputs a deterministic number based on the key.
     * 
     * @return A deterministic number, that came from the entropy-based random seed.
     */
    uint64_t next();

    /**
     * @author Alejandro Anglada Álvarez
     * @date 04-06-2026
     * 
     * @brief Returns a value inbetween the selected range.
     * 
     * @param min Lower bound of the range.
     * @param max Higher bound of the range.
     * 
     * @return A number in the range (min, max].
     */
    uint64_t range(int min, int max);
};

#endif