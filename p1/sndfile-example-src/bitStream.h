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
        fOut.open(fName, std::ios::binary | std::ios_base::out | std::fstream::trunc);
    }
    
    int read_bit(){
        if(bit_pos == -1){
            if(!fIn.is_open() || fIn.eof() || fIn.fail()){
                std::cout << "END OF FILE" << std::endl;
                return '-';
            }
            //stored = 'p'; //this example works
            fIn.read(&stored, sizeof(char));
            std::cout << stored << std::endl;
            this->b = std::bitset<8>(stored);
            std::cout << b << std::endl;
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



    int* read_Nbit(int n){
        int bit;
        int *bits;
        for(int i = 0; i < n; i++){
            bit = read_bit();
            std::cout << bit << std::endl;
            bits[i] = bit;
        }
        return bits;
    }


    void write_Nbit(char *array){
        int bits_to_flush = sizeof(array) % 8; // numero de bits que nao sao suficientes para formar 1 byte
        int cont_bits = 0;

        for(int i = sizeof(array)-bits_to_flush; i > 0; i++){  // escrever o byte para o ficheiro
            if(array[i] == '1'){
                write_byte.set(i%8);
            }
            cont_bits++;
            if (cont_bits == 8){
                write_to_file();
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
            write_to_file();
            write_byte.reset();
            cont_bits = 0;
        }
    }

    void write_to_file(){
        unsigned long i = to_write_bits.to_ulong();
        const char c = static_cast<unsigned char>( i ); // simplest -- no checks for 8 bit bitsets

        fOut.write(&c, sizeof(char));
        //std::cout << c << std::endl;
    }
};

#endif
