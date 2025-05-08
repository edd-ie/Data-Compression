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

    const std::string read = "../Resources/tiny.txt";
    const std::string save = "lz78Tiny";
    const std::string write = "lz78Decomp";

    std::cout << "Hello: " << static_cast<unsigned char>(1) << std::endl;
    const auto test3 = std::make_unique<LZ78Compressor>();
    test3->encode(read);

    if(test3->save(save, 0)) {
        const auto test4 = std::make_unique<LZ78Compressor>();
        test4->load(save, 0);
        if (test4->decode(write))
            std::cout << "\nWrite was successful\n";
    }

    return 0;
}