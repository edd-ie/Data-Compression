#include <iostream>
#include <memory>
#include <limits>
#include "Lossless/LempelZiv/LZ77.h"

int main() {

    // const auto test1 = std::make_unique<LZ77Compressor>();
    // test1->encode("../Resources/small.txt");
    // test1->save("encodeSmall",0);

    // const auto test2 = std::make_unique<LZ77Compressor>();
    // test2->load("encodeSmall",0);
    // test2->decode("decodeSmall");

    std::cout << "Maximum value for uint16_t: " << static_cast<unsigned char>(1) << std::endl;

    return 0;
}