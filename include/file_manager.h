#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <fstream>
#include <string>

class FileManager{
private:

    std::fstream _json;
    std::string _path;
    bool _isOpen;

    /**
     * @author Alejandro Anglada Álvarez
     * @date 04-06-2026
     * 
     * @brief FileManager class' initializer. It's called in the FileManager's constructor.
     * 
     * @param path Path to the file.
     * 
     * @post The object associated to the FileManager's instance has been initialized succesfuly.
     */
    void initialize(std::string path);

    /**
     * @author Alejandro Anglada Álvarez
     * @date 04-06-2026
     * 
     * @brief Opens the file in the given path. If it's not found, it creates a new JSON file.
     * 
     * @param path The path in which the file resides.
     * 
     * @pre The instance of FileManager that calls this method must be valid.
     * 
     * @return true if it has been opened/created succesfuly, false otherwise.
     */
    bool open(std::string path);

public:

    /**
     * @author Alejandro Anglada Álvarez
     * @date 04-06-2026
     * 
     * @brief FileManager class' constructor. Creates a valid instance of a FileManager type.
     * 
     * @param name File's name. If it's not found in the current scope, it gets created.
     * 
     * @post The object associated to the FileManager's instance has been created succesfuly.
     */
    FileManager(std::string name);

    /**
     * @author Alejandro Anglada Álvarez
     * @date 06-06-2026
     * 
     * @brief FileManager class' destructor. Safely destroys the instance.
     * 
     * @post The object associated to the FileManager's instance has been destroyed succesfuly.
     */
    ~FileManager();

    /**
     * @author Alejandro Anglada Álvarez
     * @date 04-06-2026
     * 
     * @brief Closes the fstream item.
     * 
     * @return true if it has been closed succesfuly; false otherwise.
     */
    bool close();

    /**
     * @author Alejandro Anglada Álvarez
     * @date 06-06-2026
     * 
     * @brief Check if the current fstream item is open or not.
     * 
     * @return true if the current fstream item is open; false otherwise.
     */
    bool isOpen() const;

    /**
     * @author Alejandro Anglada Álvarez
     * @date 06-06-2026
     * 
     * @brief Saves a new pair key-password in the JSON file.
     * 
     * @param key Identifier.
     * @param pwd Password.
     * 
     * @return true if it has been added succesfuly, false otherwise.
     * 
     * @pre Both the key and the pwd must be initialized correctly.
     * @post The pair key-pwd is added to the JSON file open in the fstream item.
     */
    bool savePassword(std::string key, std::string pwd);

    /**
     * @author Alejandro Anglada Álvarez
     * @date 06-06-2026
     * 
     * @brief Checks if a given JSON content string has a valid format (i.e. all his { close).
     * 
     * @param jsonContent Contents of the JSON file.
     * 
     * @return true if it has a valid format; false otherwise.
     */
    static bool isValidJson(std::string jsonContent);

};

#endif