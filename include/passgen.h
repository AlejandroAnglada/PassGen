#ifndef PASSGEN_H
#define PASSGEN_H

#include <string>
#include "key.h"

class PassGen{
private:
    Key _key;
    bool _num;
    bool _symbols;

    /**
     * @author Alejandro Anglada Álvarez
     * @date 04-06-2026
     * 
     * @brief _key attribute's getter.
     * 
     * @return _key.
     */
    Key& getKey();

public:
    /**
     * @author Alejandro Anglada Álvarez
     * @date 04-06-2026
     * 
     * @brief PassGen constructor. Creates a valid instance of a 
     * PassGen object.
     * 
     * @param num Determines if the password will contain or not numbers. Defaults to true.
     * @param symbols Determines if the password will contain or not symbols. Defaults to true.
     * 
     * @post The object PassGen will be created with the parameters specified.
     */
    PassGen(bool num = true, bool symbols = true);

    /**
     * @author Alejandro Anglada Álvarez
     * @date 04-06-2026
     * 
     * @brief _num attribute's getter.
     * 
     * @return _num.
     */
    bool getNum() const;

    /**
     * @author Alejandro Anglada Álvarez
     * @date 04-06-2026
     * 
     * @brief _symbols attribute's getter.
     * 
     * @return _symbols.
     */
    bool getSymbols() const;

    /**
     * @author Alejandro Anglada Álvarez
     * @date 04-06-2026
     * 
     * @brief Generates a secure, entropy-based password of the given length.
     * 
     * @param len Desired length of the password.
     * 
     * @post A string containing the randomly, entropy-based generated password will be created.
     */
    std::string generate(size_t len);
};

#endif