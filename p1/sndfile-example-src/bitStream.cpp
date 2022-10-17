#include <fstream>
#include <bitset>
#include "bitStream.h"

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

    return 0;
}