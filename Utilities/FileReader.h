//
// Created by _edd.ie_ on 13/04/2025.
//

#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "Logger.h"

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::ostringstream;
using std::string;

// reads the file contents and store them in a std::string object
inline string readFileIntoString(const string& path) {
    ifstream input_file(path);
    if (!input_file.is_open()) {
        Logger::log(1, "%s: Could not open the file - %s\n", __FUNCTION__, path);
        throw std::runtime_error("Could not open file: " + path);
    }
    std::stringstream ss;
    ss << input_file.rdbuf();
    return ss.str();
}

// Convert string to a double
inline double stringToDouble(const std::string& str) {
    std::stringstream ss(str);
    double result;

    if (ss >> result) {
        // Check if there are any remaining characters after the double was read.
        char remaining;
        if (ss >> remaining) {
            // There were extra characters, so the conversion failed.
            Logger::log(1, "%s: Error with string to double conversion.\n", __FUNCTION__);
            throw std::invalid_argument("String contains non-numeric characters after the double.");
        }
        return result;
    }
    // The string could not be converted to a double.
    Logger::log(1, "%s: Error with string to double conversion.\n", __FUNCTION__);
    throw std::invalid_argument("String is not a valid double.");
}


// Generic function to save a vector of any type to a text file
template <typename T>
void saveVectorToTextFile(const std::vector<T>& vec, const std::string& filename) {
    std::ofstream outputFile(filename);
    if (outputFile.is_open()) {
        for (size_t i = 0; i < vec.size(); ++i) {
            outputFile << vec[i] << (i == vec.size() - 1 ? "" : " ");
        }
        outputFile << std::endl;
        outputFile.close();
        Logger::log(2, "%s: Vector of type %s successfully saved to %s\n", __FUNCTION__, typeid(T).name(), filename.c_str());
    } else {
        Logger::log(0, "%s: Error opening file for writing: %s\n", __FUNCTION__, filename.c_str());
    }
}

// Generic function to load a vector of any type from a text file
template <typename T>
std::vector<T> loadVectorFromTextFile(const std::string& filename) {
    std::vector<T> loadedVector;
    std::ifstream inputFile(filename);
    if (inputFile.is_open()) {
        std::string line;
        if (std::getline(inputFile, line)) {
            std::stringstream ss(line);
            T value;
            while (ss >> value) {
                loadedVector.push_back(value);
            }
            Logger::log(2, "%s: Vector of type %s successfully loaded from %s\n", __FUNCTION__, typeid(T).name(), filename.c_str());
        } else {
            Logger::log(1, "%s: Error reading line from file: %s\n", __FUNCTION__, filename.c_str());
        }
        inputFile.close();
    } else {
        Logger::log(0, "%s: Error opening file for reading: %s\n", __FUNCTION__, filename.c_str());
    }
    return loadedVector;
}

// Generic function to save a vector of any type to a binary file
template <typename T>
void saveVectorToBinaryFile(const std::vector<T>& vec, const std::string& filename) {
    std::ofstream outputFile(filename, std::ios::binary);
    if (outputFile.is_open()) {
        size_t size = vec.size();
        outputFile.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
        if (!vec.empty()) {
            outputFile.write(reinterpret_cast<const char*>(vec.data()), size * sizeof(T));
        }
        outputFile.close();
        Logger::log(2, "%s: Vector of type %s successfully saved to %s (binary)\n", __FUNCTION__, typeid(T).name(), filename.c_str());
    } else {
        Logger::log(0, "%s: Error opening binary file for writing: %s\n", __FUNCTION__, filename.c_str());
    }
}

// Generic function to load a vector of any type from a binary file
template <typename T>
std::vector<T> loadVectorFromBinaryFile(const std::string& filename) {
    std::vector<T> loadedVector;
    std::ifstream inputFile(filename, std::ios::binary);
    if (inputFile.is_open()) {
        size_t size;
        if (inputFile.read(reinterpret_cast<char*>(&size), sizeof(size_t))) {
            loadedVector.resize(size);
            if (size > 0) {
                if (!inputFile.read(reinterpret_cast<char*>(loadedVector.data()), size * sizeof(T))) {
                    Logger::log(1, "%s: Error reading data of type %s from binary file: %s\n", __FUNCTION__, typeid(T).name(), filename.c_str());
                    loadedVector.clear();
                } else {
                    Logger::log(2, "%s: Vector of type %s successfully loaded from %s (binary)\n", __FUNCTION__, typeid(T).name(), filename.c_str());
                }
            } else {
                Logger::log(2, "%s: Loaded an empty vector of type %s from %s (binary)\n", __FUNCTION__, typeid(T).name(), filename.c_str());
            }
        } else {
            Logger::log(1, "%s: Error reading vector size from binary file: %s\n", __FUNCTION__, filename.c_str());
        }
        inputFile.close();
    } else {
        Logger::log(0, "%s: Error opening binary file for reading: %s\n", __FUNCTION__, filename.c_str());
    }
    return loadedVector;
}

#endif //FILEREADER_H
