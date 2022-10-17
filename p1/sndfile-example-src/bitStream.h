#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <bitset>

class BitStream {
private:
    int write_pos = 0;
    int bit_read = 9;
    std::ifstream fIn;
    std::ofstream fOut;
    std::bitset<8> bits_write;
    std::bitset<8> bits_read;

    void write_to_file() {
        unsigned long char_value = bits_write.to_ulong();
        const char c = static_cast<char>( char_value );
        fOut.write(&c, sizeof(char));
        write_pos = 0;
    };

    void flush_bits() {
        write_to_file();
    }

public:
    BitStream(const char *fnameIn, const char *fnameOut) {
        fIn = std::ifstream(fnameIn, std::istream::binary);
        fOut = std::ofstream(fnameOut, std::ostream::binary | std::ostream::trunc);
    }

    int read_bit() {
        // all bits of the byte were consumed
        if (bit_read > 7) {
            // read one more byte
            char byte;


            if (fIn.eof() || fIn.fail() || fIn.bad() || !fIn.is_open()) {
                return -1;
            }

            fIn.read(&byte, sizeof(char));
            bits_read = std::bitset<8>((int)byte);
            bit_read = 0;
        }

        return bits_read[bit_read++];
    }

    void write_bit(int bit) {

        if (write_pos > 7) {
            write_to_file();
            write_pos = 0;
        }
        bits_write.set(7-write_pos, bit == 1);
        write_pos++;
    }

    void close_files() {
        if (write_pos >0)
            flush_bits();

        fIn.close();
        fOut.close();
    }

};

#endif
