cmake_minimum_required(VERSION 3.30)
project(Data_Compression)

set(CMAKE_CXX_STANDARD 23)

add_executable(Data_Compression main.cpp
        Utilities/Logger.h
        Utilities/Logger.cpp
        Utilities/FileReader.cpp
        Utilities/FileReader.h
        Lossless/LempelZiv/LZ77.h
        Lossless/LempelZiv/LZ77.cpp
        Lossless/LempelZiv/LZ78.h
        Lossless/LempelZiv/LZ78.cpp
)
