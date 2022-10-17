#include <fstream>
#include <bitset>
#include "bitStream.h"
#include <string>

using namespace std;
char stored;

//00100101
int main() {
    string str = "0111000001100101011001000111001001101111001000000110110001101111011100000110010101110011001000000011000100110010001100110000101000001010";

    bitset<8> char_enc;
    BitStream bEncode{"", "str_decoded.txt"};
    for (size_t i = 0; i < str.size(); ++i) {
        bEncode.write_bit(str[i] == '1');
    }

    bEncode.close_files();
    BitStream bDecode{"str_decoded.txt", ""};
    ofstream fWrite{"str_bin.txt", ofstream::app};
    int bit = bDecode.read_bit();
    while (bit != -1) {
        fWrite << bit;
        bit = bDecode.read_bit();
    }
    return 0;

}

