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
#include <sndfile.hh>
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
    int bit_pos = 0;
    int write_pos = 0;
    int read_bytes = 0;
    std::bitset<8> to_write_bits;
    std::bitset<8> write_byte;
    int written_bits_n = 0;


public:

    BitStream(std::string fName){
        fIn.open(fName, std::ios::binary | std::ios_base::in | std::ios_base::out);
        fOut.open(fName, std::ios::binary | std::ios_base::out | std::fstream::app);
    }
    
    int read_bit(){
        if(!fIn.is_open() || fIn.peek() == EOF || fIn.eof()){
            return '-';
        }
        this->stored = fIn.get();
        this->b = std::bitset<8>(stored);
        if(bit_pos > 8){
            this->bit_pos = 0;
            this->read_bytes++;
        }
        return b[bit_pos++];
    }

    
    void write_bit(char bit){
        if(bit == '1'){
            to_write_bits.set(write_pos);
        }

        write_pos++;

        if(write_pos == 8){
        
            write_to_file(to_write_bits);        
        
            to_write_bits.reset();

            write_pos = 0;
        }
    }

    void close_files() {
        fOut.close();
        fIn.close();
    }



    void read_Nbit(int n){
        for(int i = 0; i < n; i++){
            read_bit();
        }
    }


    void write_Nbit(char array[]){
        int bits_to_flush = sizeof(array) % 8; // numero de bits que nao sao suficientes para formar 1 byte
        int cont_bits = 0;

        for(int i = 0; i < sizeof(array)-bits_to_flush; i++){  // escrever o byte para o ficheiro
            if(array[i] == '1'){
                write_byte.set(i%8);
            }
            cont_bits++;
            if (cont_bits == 8){
                write_to_file(write_byte);
                write_byte.reset();
                cont_bits = 0;
            }
        }

        while(bits_to_flush != 0){ // envia o resto dos bits o resto permanece a '0' o default do bitset
            if(array[sizeof(array) - bits_to_flush] == '1'){
                write_byte.set(cont_bits);
                cont_bits++;
            }
            bits_to_flush--;
        }
        if (cont_bits != 0){ // só escreve no ficheiro se houver bits para dar flush
            write_to_file(write_byte);
            write_byte.reset();
            cont_bits = 0;
        }
    }

    void write_to_file(std::bitset<8> to_write_bits){
        //unsigned long i = to_write_bits.to_ulong();
        //const char c = static_cast<unsigned char>( i ); // simplest -- no checks for 8 bit bitsets

        std::string byt = to_write_bits.to_string();
        char array[9] = {0};

        std::copy(byt.begin(), byt.end(), array);


        fOut.write(array, 8);
        std::cout << array;
    }
};

#endif
