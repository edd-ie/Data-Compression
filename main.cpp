#include <iostream>
#include <memory>

#include "Lossless/LempelZiv/LZ77.h"

int main() {
    // std::cout << "Enter file name" << std::endl;
    // std::string fileName;
    // std::cin >> fileName;

    const auto test1 = std::make_unique<LZ77Compressor>();
    test1->encode("../Resources/small.txt");
    test1->printData();
    // test1->save("outTiny",0);

    // const auto test2 = std::make_unique<LZ77Compressor>();
    // test2->load("small",0);
    // test2->printData();

    return 0;
}