#include <iostream>
// Para generar números pseudoaleatorios
#include <cstdlib>
// Para que los números sean determinísticos con respecto a la ejecución actual del subproceso,
// y no determinístcos con respecto al binario.
#include <ctime>

class Generator{
private:
    u_int32_t _seed = 0;

    /**
     * @author Alejandro Anglada Álvarez
     * @date 04-06-2026
     * 
     * @brief Sets this object's seed to the parameter.
     * 
     * @param s New seed.
     * 
     * @pre The u_int32_t must represent a safe, pseudo-randomly generated seed.
     * @post This object's _seed is now set to s.
     * 
     * @return true if the _seed value was changed correctly, false otherwise.
     */
    bool setSeed(u_int32_t s);

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
     * @pre The seed must be a valid instance of a u_int32_t data.
     * @post The newly-created Generator object has a _seed set to seed parameter.
     */
    Generator(u_int32_t seed);

    /** 
     * @author Alejandro Anglada
     * @date 04-06-2026
     * 
     * @brief Returns the current seed value.
     * 
     * @return The current seed value.
    */
    u_int32_t getSeed();
};