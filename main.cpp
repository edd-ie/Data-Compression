#include <iostream>
#include <memory>
#include <limits>
#include "Lossless/LempelZiv/LZ77.h"
#include "Lossless/LempelZiv/LZ78.h"

int main() {

    // const auto test1 = std::make_unique<LZ77Compressor>();
    // test1->encode("../Resources/small.txt");
    // test1->save("encodeSmall",0);

    // const auto test2 = std::make_unique<LZ77Compressor>();
    // test2->load("encodeSmall",0);
    // test2->decode("decodeSmall");

    std::cout << "Maximum value for uint16_t: " << static_cast<unsigned char>(1) << std::endl;
    const auto test3 = std::make_unique<LZ78Compressor>();
    test3->encode("../Resources/small.txt");

    if(test3->save("lz78Small", 0)) {
        const auto test4 = std::make_unique<LZ78Compressor>();
        test4->load("lz78Small", 0);
        test4->printData();
    }

    return 0;
}