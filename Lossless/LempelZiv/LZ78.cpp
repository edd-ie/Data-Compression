//
// Created by _edd.ie_ on 07/05/2025.
//

#include "LZ78.h"
#include "../../Utilities/Logger.h"
#include "../../Utilities/FileReader.h"
#include <fstream>

bool LZ78Compressor::encode(const std::string &fileName) {
    std::ifstream file(fileName); // Open the file
    if (!file.is_open()) {
        Logger::log(1, "%s: Failed to read file: %s\n", __FUNCTION__, fileName);
        return false;
    }

    char letter;
    std::string currentString;
    unsigned short nextIndex = 1;
    while (file.get(letter)) { // Read character by character
        //std::string extendedString = currentString + character;
        std::string extendedString;
        extendedString.reserve(currentString.size() + 1); // Pre-allocate memory
        extendedString = currentString + letter;

        // Check if the extended string is in the dictionary
        if (symbolTable.contains(extendedString)) {
            currentString = extendedString; // Extend the current string
        } else {
            // Output the (index, character) pair
            unsigned short index = (currentString.empty()) ? 0 : symbolTable[currentString];
            data.emplace_back(index, letter);

            // Add the extended string to the dictionary
            symbolTable[extendedString] = nextIndex;
            nextIndex++;

            currentString = ""; // Reset the current string
        }
    }

    file.close(); // Close the file
    return true;
}

void LZ78Compressor::printData() const {
    std::cout << "Compressed dataset:\n";
    for (const LZ78 x : data) {
        printf("{%d, %c}\n", x.index, x.next);
    }
}


bool LZ78Compressor::save(const std::string &fileName, const int fileType) const {
    try {
        if (fileType==0)
            saveVectorToBinaryFile(data, "../Resources/"+fileName+".bin");
        else
            saveVectorToTextFile(data, "../Resources/"+fileName+".txt");
        return true;
    }
    catch (const std::exception& e) {
        Logger::log(1, "%s: Error Exporting data: %s\n", __FUNCTION__, e.what());
        return false;
    }
}

bool LZ78Compressor::load(const std::string &fileName, const int fileType) {
    try {
        data = (fileType==0) ?
            loadVectorFromBinaryFile<LZ78>("../Resources/"+fileName+".bin") :
            loadVectorFromTextFile<LZ78>("../Resources/"+fileName+".txt");
        return true;
    }
    catch (const std::exception& e) {
        Logger::log(1, "%s: Error importing data: %s\n", __FUNCTION__, e.what());
        return false;
    }
}