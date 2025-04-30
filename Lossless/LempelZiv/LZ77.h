//
// Created by _edd.ie_ on 29/04/2025.
//

#ifndef LZ77_H
#define LZ77_H
#include <string>
#include <vector>
#include <iostream>

#define BIN 0
#define TXT 1

struct LZ77 {
    unsigned short distance;
    unsigned short length;
    char next;

    // Default constructor
    LZ77() : distance(0), length(0), next('\0') {}

    LZ77(const unsigned short distance, const unsigned short length, const char next) :
        next(next), distance(distance), length(length) {}

    // Friend function to overload the output stream operator
    friend std::ostream& operator<<(std::ostream& os, const LZ77& lz77) {
        os << lz77.distance << " " << lz77.length << " " << lz77.next;
        return os;
    }

    // Friend function to overload the input stream operator
    friend std::istream& operator>>(std::istream& is, LZ77& lz77) {
        if (is >> lz77.distance >> lz77.length >> lz77.next) {
            return is;
        }
        // If reading fails, set the failbit of the input stream
        is.setstate(std::ios::failbit);
        return is;
    }
};

class LZ77Compressor {
    std::vector<LZ77> data;
    unsigned short searchBufferSize{};
public:
    LZ77Compressor()= default;
    bool encode(const std::string& fileName);
    bool decode(const std::string& fileName);
    bool save(const std::string& fileName, int fileType);
    bool load(const std::string& fileName, int fileType);
    void printData();
};

#endif //LZ77_H
