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

    std::unordered_map<std::string, unsigned short> symbolTable;
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


bool LZ78Compressor::decode(const std::string& fileName) const {
    try {
        std::ofstream file("../Resources/"+fileName+".txt", std::ios::out | std::ios::binary);
        if (!file) {
            Logger::log(1, "%s: Error creating the file: %s\n", __FUNCTION__, fileName);
            return false;
        }

        // Buffer to store decoded text and handle back-references
        std::string decodedBuffer;
        unsigned short pos = 1;
        std::unordered_map<unsigned short, std::string> dictionary; // Local dictionary

        for (const auto& token : data) {
            std::string entry; // Use a separate variable for the dictionary entry
            if (token.index == 0) {
                entry = token.next;
                dictionary[pos++] = entry; // Use pos as key, *before* incrementing.
                file.put(token.next);
            } else {
                // Handle the case where token.index is out of range.
                if (token.index > dictionary.size())
                {
                    Logger::log(1, "%s: Error: token.index is out of range.  token.index = %d, dictionary.size() = %d\n", __FUNCTION__, token.index, (int)dictionary.size());
                    return false;
                }
                entry = dictionary[token.index];
                entry += token.next;
                dictionary[pos++] = entry;
                for (char x : entry)
                    file.put(x);
            }
        }

        file.close(); // Explicitly close the file

        // Check if any errors occurred during writing
        if (file.fail()) {
            Logger::log(1, "%s: Error writing to file: %s\n", __FUNCTION__, fileName);
            return false;
        }

        return true;
    } catch (const std::exception& e) {
        Logger::log(1, "%s: Error exporting data: %s\n", __FUNCTION__, e.what());
        return false;
    }
}
