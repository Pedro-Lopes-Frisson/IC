#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <bitset>
#include <string>
#include <bitset>
#include <string>
//#include <sndfile.hh>
struct Student {
    int roll_no;
    std::string name;
};

class BitStream {
private:
    std::fstream fIn;
    std::fstream fOut;
    char stored;
    std::bitset<8> b;
    int bit_pos = 9;
    int read_bytes = 0;
    std::bitset<8> to_write_bits;
    int written_bits_n = 0;


public:
    BitStream(std::string fName) {
        fIn.open(fName, std::ios::binary | std::ios_base::in | std::ios_base::out);
        fOut.open(fName, std::ios::binary | std::ios_base::out | std::fstream ::app);
    }

    int read_bit() {
        if (!fIn.is_open() || fIn.peek() == EOF) {
            return '-';
        }
        if (bit_pos > 8) {
            this->stored = fIn.get();
            this->b = std::bitset<8>(stored);
            this->bit_pos = 0;
            this->read_bytes++;
        }

        return b[bit_pos++];
    }


    void write_bit(int bit) {
        to_write_bits.set(written_bits_n , bit);
        written_bits_n++;

        if (written_bits_n >= 8) {
            unsigned long i = to_write_bits.to_ulong();
            const char c = static_cast<unsigned char>( i ); // simplest -- no checks for 8 bit bitsets

            fOut.write(&c, 8);
            std::cout << c;

            written_bits_n = 0;
            to_write_bits = std::bitset<8>(0);
        }
    }

    void close_files() {
        fOut.close();
        fIn.close();
    }



    //read_Nbit(){
    //}


    //write_Nbit(){
    //}
};

#endif
