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
        fIn.open(fName, std::ios::binary | std::ios_base::in);
        fOut.open(fName, std::ios::binary | std::ios_base::out | std::fstream::app);
    }

    char read_bit() {
        if (fIn.eof() || !fIn.is_open() ){
            return '-';
        }

        char c;
        fIn.get(c);
        return c;
    }


    void write_bit(char bit) {
        to_write_bits.set(7 - written_bits_n, bit == '1');
        written_bits_n++;

        if (written_bits_n >= 8) {
            unsigned long i = to_write_bits.to_ulong();
            const char c = static_cast<unsigned char>( i ); // simplest -- no checks for 8 bit bitsets
            fOut.write(&c, sizeof(char));

            written_bits_n = 0;
            to_write_bits.reset();
        }
    }

    void close_files() {
        //  bits que ainda tem para escrever
        if(written_bits_n > 0){

            unsigned long i = to_write_bits.to_ulong();
            const char c = static_cast<unsigned char>( i ); // simplest -- no checks for 8 bit bitsets
            fOut.write(&c, sizeof(char));

        }
        fOut.close();
        fIn.close();
    }


    void read_Nbit(char *array, int n) {
        char bit = read_bit();
        int count = 0;

        std::cout << "Count " << count << " Bit" << bit << std::endl;
        while (count < n && bit != '-' ) {
            std::cout << "Count " << count << " Bit" << bit << std::endl;
            array[count] = bit;
            bit = read_bit();
            count++;
        }
        std::cout << "AAAAAAAAAAAAAA" << bit << "|";
    }


    void write_Nbit(char *array, int n){
        int count = 0;
        while(count < n){
            write_bit(array[count]);
            count++;
        }

    }
};

#endif
