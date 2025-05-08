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

        unsigned short current_pos = 0;

        searchBufferSize = 0;

        while (current_pos < fileContents.length()) {
            unsigned short longest_match_length = 0;
            unsigned short longest_match_distance = 0;

            // Define the boundaries of the search buffer
            unsigned short search_start = std::max(0, (int)current_pos - max_search_buffer_size);
            unsigned short search_end = current_pos;

            // Iterate through all possible starting positions in the search buffer
            for (unsigned short i = search_start; i < search_end; ++i) {
                unsigned short current_match_length = 0;
                unsigned short lookahead_pos = current_pos;
                unsigned short search_pos = i;

                // Compare characters in the search and lookahead buffers
                while (lookahead_pos < fileContents.length() &&
                    search_pos < search_end &&
                    fileContents[lookahead_pos] == fileContents[search_pos]) {
                        current_match_length++;
                        lookahead_pos++;
                        search_pos++;
                    }

                // Update the longest match if a longer one is found
                if (current_match_length > longest_match_length) {
                    longest_match_length = current_match_length;
                    longest_match_distance = current_pos - i; // Distance is the offset backwards
                }
            }

            if (longest_match_length > 0) {
                data.emplace_back(longest_match_distance, longest_match_length,
                (current_pos + longest_match_length < fileContents.length()) ?
                    fileContents[current_pos + longest_match_length] : '\0');
                current_pos += longest_match_length + 1; // Move past the matched sequence and the next char
            } else {
                data.emplace_back(0, 0, fileContents[current_pos]);
                current_pos++;
            }
        }
        return true;
    } catch (const std::exception& e) {
        Logger::log(1, "%s: Error importing data: %s\n", __FUNCTION__, e.what());
        return false;
    }
}

void LZ77Compressor::printData() {
    std::cout << "Compressed dataset:\n";
    for (LZ77 x : data) {
        printf("{%d, %d, %c}\n", x.distance, x.length, x.next);
    }
}

bool LZ77Compressor::save(const std::string &fileName, const int fileType) {
    try {
        if (fileType==BIN)
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

bool LZ77Compressor::load(const std::string &fileName, const int fileType) {
    try {
        data = (fileType==BIN) ?
            loadVectorFromBinaryFile<LZ77>("../Resources/"+fileName+".bin") :
            loadVectorFromTextFile<LZ77>("../Resources/"+fileName+".txt");
        return true;
    }
    catch (const std::exception& e) {
        Logger::log(1, "%s: Error importing data: %s\n", __FUNCTION__, e.what());
        return false;
    }
}

bool LZ77Compressor::decode(const std::string &fileName) {
    try {
        std::ofstream file("../Resources/"+fileName+".txt", std::ios::out | std::ios::binary);
        if (!file) {
            Logger::log(1, "%s: Error creating the file: %s\n", __FUNCTION__, fileName);
            return false;
        }

        // Buffer to store decoded text and handle back-references
        std::string decodedBuffer;
        decodedBuffer.reserve(data.size() * 2); // Reserve some space to minimize reallocations

        for (const auto& token : data) {
            // If we have a back reference (length > 0)
            if (token.length > 0) {
                // Calculate the start position for copying
                size_t startPos = decodedBuffer.length() - token.distance;

                // Copy the referenced sequence
                for (unsigned short i = 0; i < token.length; ++i) {
                    char c = decodedBuffer[startPos + i];
                    decodedBuffer.push_back(c);
                    file.put(c);
                }
            }

            // Add the next character (if it's not null terminator)
            if (token.next != '\0') {
                decodedBuffer.push_back(token.next);
                file.put(token.next);
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
