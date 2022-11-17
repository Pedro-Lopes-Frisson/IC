#ifndef P2_DECODER_H
#define P2_DECODER_H

#include "bitStream.h"
#include "GolombCoding.h"
#include <string>
#include <sndfile.hh>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>

class Decoder {
private:
	GolombCoder coder{};

	void decode_residual(short value, std::string &bits) {
	  	int sucss;
	    sucss = coder.decode_int(value, bits);

	    if(sucss != 0){
	    	std::cerr << "Sample decoding could not be performed!" << std::endl;
	    }
	};

	void read_from_file(BitStream &bitStream){

	}




public:
	Decoder (){

	}

	// inFile é o ficheiro codificado - outFile é o ficheiro descodificado
	void decode_Encoded_audio_file(std::string inFile, std::string outFile){

		// Abrir o ficheiro codificado
		BitStream bitStream = BitStream{outFile};
		
		// Para ler os bits do k
		bitset<8> read_k_bits = bitset<8>(0);

		// Buscar o k
		for (int j = 0; j < 9; ++j) {
			if(bitStream.read_bit() == 0){
				break;
			}
			else{
				read_k_bits[j] = bitStream.read_bit();
			}

    	}
    	// Tranformar o k em um inteiro
    	std::string k;
    	for (auto b : read_k_bits){
    		k = k + std::to_string(b);
    	}

    	// Obter o M
    	int M = pow(2, k);
    	// Para ler os bits do q e do r
    	bitset<std::to_string(k)> read_qr_bits
    	// Para guardar os bits do q e r em string para depois usar o decode_int do golomb
    	std::string str_qr;

	}




};

#endif //P2_DECODER_H