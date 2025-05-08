//
// Created by _edd.ie_ on 07/05/2025.
//

#ifndef LZ78_H
#define LZ78_H

#include <iostream>
#include <unordered_map>
#include <vector>

struct LZ78 {
    unsigned short index;
    char next;

    // Default constructor
    LZ78() : index(0), next('\0') {}

    LZ78(const unsigned short index, const char next) :
        next(next), index(index){}

    // Friend function to overload the output stream operator
    friend std::ostream& operator<<(std::ostream& os, const LZ78& lz78) {
        os << lz78.index << " " << lz78.next;
        return os;
    }

    // Friend function to overload the input stream operator
    friend std::istream& operator>>(std::istream& is, LZ78& lz78) {
        if (is >> lz78.index >> lz78.next) {
            return is;
        }
        // If reading fails, set the fail bit of the input stream
        is.setstate(std::ios::failbit);
        return is;
    }
};

class LZ78Compressor {
    std::vector<LZ78>data;
    std::unordered_map<unsigned short, std::string> dictionary;
    std::unordered_map<std::string, unsigned short> symbolTable;

public:
    LZ78Compressor()= default;
    bool encode(const std::string& fileName);
    bool decode(const std::string& fileName);
    bool save(const std::string& fileName, int fileType) const;
    bool load(const std::string& fileName, int fileType);
    void printData() const;
};


#endif //LZ78_H
