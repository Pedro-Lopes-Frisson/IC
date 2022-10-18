#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <fstream>
#include <iostream>
#include <ios>
#include <vector>
#include <map>
#include <bitset>
#include <string>
#include <sndfile.hh>
struct Student {
    int roll_no;
    std::string name;
};

class BitStream {
private:
    std::ifstream fIn;
    std::fstream fOut;
    std::ifstream fDec;
    std::bitset<8> b;
    char stored;   
    int bit_pos = -1;
    int write_pos = 0;
    int read_bytes = 0;
    std::bitset<8> to_write_bits;
    std::bitset<8> write_byte;
    int written_bits_n = 0;
    

public:

    BitStream(std::string fName){
        fIn.open(fName, std::ios::binary );
        fOut.open(fName + "enc", std::ios::binary | std::ios_base::out | std::fstream::trunc);
    }
    
    int read_bit(){
        if(bit_pos == -1){
            if(fIn.eof() || fIn.fail() || fIn.bad() || !fIn.is_open()){
                std::cout << "END OF FILE" << std::endl;
                return '-';
            }
            fIn.read(&stored, sizeof(char));
            this->b = std::bitset<8>(stored);
            this->bit_pos = 7;
            this->read_bytes++;
        }
        return b[bit_pos--];
    }

    
    void write_bit(char bit){
        if(bit == '1'){
            to_write_bits.set(7-write_pos);
        }

        write_pos++;

        if(write_pos == 8){
        
            write_to_file();        
        
            to_write_bits.reset();

            write_pos = 0;
        }
    }

    void close_files() {
        if(write_pos != 0){
            write_to_file();
        }
        fOut.close();
        fIn.close();
    }



    std::vector<char> read_Nbit(int n){
        int b;
        char bit;
        std::vector<char> bits;
        for(int i = 0; i < n; i++){
            b = read_bit();
            bit = b;
            bit += 48; // ASCII code of 0 or 1
            bits.push_back(bit);
        }
        return bits;
    }

    void write_Nbit(int n){
        int bits_to_flush = n % 8;
        int flush = 0;
        char bit;
        for(int i = 0; i < (n-bits_to_flush); i++){
            fIn.read(&bit, sizeof(char));
            write_bit(bit);
        }
        while(bits_to_flush != 0){
            fIn.read(&bit, sizeof(char));
            write_bit(bit);
            bits_to_flush--;
            flush = 1;
        }
        if(flush)
            write_to_file();
    }

    void write_to_file(){
        unsigned long i = to_write_bits.to_ulong();
        const char c = static_cast<unsigned char>( i ); // simplest -- no checks for 8 bit bitsets

        fOut.write(&c, sizeof(char));
    }
};

#endif
