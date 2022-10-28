#include <fstream>
#include <bitset>
#include "bitStream.h"
#include <string>

using namespace std;

//00100101
int main() {
    //Encoder
    ifstream fIn("pedro_bin.txt");
    BitStream b = BitStream("pedro.txt");
    char c;
    while (fIn.read(&c, sizeof(char))) {
        b.write_bit(c);
    }

    //Decoder
    BitStream rStream = BitStream("pedro.txt");
    ofstream fOut("out_file");
    auto bit = rStream.read_bit();
    while (bit != '-') {
        //std::cout << bit << endl;
        fOut << bit;
        bit = rStream.read_bit();
    }


    b = BitStream("NBits.txt");
    bitset<16> bits = bitset<16>();
    bits.set(0,0);
    bits.set(1,1);
    bits.set(2,1);
    bits.set(3,1);
    bits.set(4,0);
    bits.set(5,0);
    bits.set(6,0);
    bits.set(7,0);
    bits.set(8,0);
    bits.set(9,1);
    bits.set(10,1);
    bits.set(11,0);
    bits.set(12,0);
    bits.set(13,1);
    bits.set(14,0);
    bits.set(15,1);
    b.write_Nbit(bits);
    b.close_files();



    return 0;
}
