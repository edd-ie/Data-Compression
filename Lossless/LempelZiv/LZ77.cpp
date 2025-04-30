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

        unsigned short longest = 0,
            longest_pos = 0,
            searchTail_pos = 0,
            searchHead_pos = 0,
            current_pos = 0,
            max_search_buffer_size = 500;

        searchBufferSize = 0;
        char* searchHead;
        char* current;
        char* searchCusor;
        char* lookAheadTail;

        // while (*(current = &fileContents[current_pos]) != '\0') {
        //     searchHead = &fileContents[searchHead_pos];
        //     searchCusor = searchHead;
        //     lookAheadTail = current;
        //
        //     if (searchBufferSize == 0) {
        //         data.emplace_back(0, 0, *current);
        //         current_pos++;
        //         searchBufferSize++;
        //     }
        //     else if (searchBufferSize > 500) {
        //         searchBufferSize = searchBufferSize/2;
        //         searchTail_pos = searchTail_pos + searchBufferSize;
        //     }
        //
        //     // Finds the longest match
        //     unsigned short counter = 0;
        //     while (searchTail_pos < searchHead_pos && *lookAheadTail != '\0') {
        //         if (*lookAheadTail == *searchCusor) {
        //             counter++;
        //             searchCusor++;
        //             lookAheadTail++;
        //         }
        //         else {
        //             if (counter > longest) {
        //                 longest_pos = searchHead_pos;
        //                 longest = counter;
        //             }
        //             searchHead_pos--;
        //             searchHead = &fileContents[searchHead_pos];
        //             searchCusor = searchHead;
        //             lookAheadTail = current;
        //             counter = 0;
        //         }
        //     }
        //
        //     if(longest_pos >0) longest_pos = current_pos-longest_pos;
        //     current_pos+=longest;
        //     data.emplace_back(longest_pos, longest, fileContents[current_pos]);
        //     longest = 0;
        //     longest_pos = 0;
        //     searchHead_pos = current_pos;
        //     current_pos++;
        //     searchBufferSize++;
        // }

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

