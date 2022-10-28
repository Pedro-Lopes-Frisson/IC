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

    BitStream(std::string fName) {
        fIn.open(fName, std::ios::binary);
        fOut.open(fName + "enc", std::ios::binary | std::ios_base::out | std::fstream::trunc);
    }

    int read_bit() {
        if (bit_pos <= -1) {
            if (fIn.eof() || fIn.fail() || fIn.bad() || !fIn.is_open()) {
                std::cout << "END OF FILE" << std::endl;
                return '-';
            }
            fIn.read(&stored, sizeof(char));
            this->b = std::bitset<8>(stored);
            this->bit_pos = 7;
            this->read_bytes++;
        }
        int idx = 7 - bit_pos;
        bit_pos--;
        return b[idx];
    }


    void write_bit(int bit) {
        if (bit == 1) {
            to_write_bits.set(7 - write_pos);
        }

        write_pos++;

        if (write_pos == 8) {

            write_to_file();

            to_write_bits.reset();

            write_pos = 0;
        }
    }

    void close_files() {
        if (write_pos != 0) {
            write_to_file();
        }
        fOut.close();
        fIn.close();
    }


    std::vector<char> read_Nbit(int n) {
        int b;
        char bit;
        std::vector<char> bits;
        for (int i = 0; i < n; i++) {
            b = read_bit();
            bit = b;
            bit += 48; // ASCII code of 0 or 1
            bits.push_back(bit);
        }
        return bits;
    }

    template<std::size_t N>
    void write_Nbit(std::bitset <N> bits) {
        int n = bits.size();
        int bits_written = 0;
        while (n > 0) {
            for (int i = n - 8; i < n; i++) {
                write_bit(bits[i]);
                bits_written++;
            }
            n = n - 8;
        }
    }

    void write_to_file() {
        unsigned long i = to_write_bits.to_ulong();
        const char c = static_cast<unsigned char>( i ); // simplest -- no checks for 8 bit bitsets

        fOut.write(&c, sizeof(char));
        to_write_bits.reset();
    }
};

#endif
