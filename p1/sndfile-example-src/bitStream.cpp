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
    
    // WRITE N BIT
    BitStream b = BitStream("pedro_bin.txt");
    int write_bits;
    cout << "How many bits to write?" << endl;
    cin >> write_bits;
    b.write_Nbit(write_bits);

    // READ N BIT
    BitStream rStream = BitStream("pedro.txt");
    ofstream fOut("out_file");
    int read_bits;
    cout << "How many bits to read?" << endl;
    cin >> read_bits;
    vector<char> bits_read = rStream.read_Nbit(read_bits);
    for (int i = 0; i < (int)bits_read.size(); i++) {
        fOut << bits_read[i];
        //cout << bits_read[i] << endl;
    }
    
    
    return 0;
}