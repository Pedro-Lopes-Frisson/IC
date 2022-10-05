#include <fstream>
#include <bitset>
#include "bitStream.h"

using namespace std;
char stored;
bitset<8> b;

int main() {
    BitStream b {"binfile.bin"};
    BitStream bO {"bin1.bin"};
    auto bit = b.read_bit();
    while (b.read_bit() != '-'){
        bO.write_bit(bit);
        bit = b.read_bit();
    }
    b.close_files();
}

