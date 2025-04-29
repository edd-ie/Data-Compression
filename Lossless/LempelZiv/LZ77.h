//
// Created by _edd.ie_ on 29/04/2025.
//

#ifndef LZ77_H
#define LZ77_H
#include <string>
#include <vector>


struct LZ77 {
    char next;
    unsigned short distance;
    unsigned short length;

    LZ77(const char next, const unsigned short distance, const unsigned short length) :
        next(next), distance(distance), length(length) {}
};

class LZ77Compressor {
    std::vector<LZ77> data;
    char* searchHead{};
    char* searchTail{};
    char* lookAheadHead{};
    char* lookAheadTail{};
    unsigned short searchBufferSize{};
    unsigned short lookAheadBufferSize{};
public:
    LZ77Compressor()= default;
    bool encode(const std::string& fileName);
    bool decode(const std::string& fileName);
    bool save(const std::string& fileName);
    void printData(const std::string& fileName);
};

#endif //LZ77_H
