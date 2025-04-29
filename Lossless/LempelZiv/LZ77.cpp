//
// Created by _edd.ie_ on 29/04/2025.
//

#include "LZ77.h"
#include "../../Utilities/Logger.h"
#include "../../Utilities/FileReader.h"

bool LZ77Compressor::encode(const std::string &fileName) {
    try {
        std::string fileContents = readFileIntoString(fileName);
        if (fileContents.empty()) {
            Logger::log(1, "%s: Failed to read file: %s\n", __FUNCTION__, fileName);
            return false;
        }

        searchHead = &fileContents[0];
        searchTail = &fileContents[0];
        lookAheadHead = &fileContents[0];
        lookAheadTail = &fileContents[0];
        searchBufferSize = 0;
        lookAheadBufferSize = 0;


        return true;
    } catch (const std::exception& e) {
        Logger::log(1, "%s: Error importing data: %s\n", __FUNCTION__, e.what());
        return false;
    }
}
