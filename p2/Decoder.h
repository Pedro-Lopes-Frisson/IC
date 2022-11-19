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
	GolombCoder coder{(int) pow(2, 16)};

	void decode_residual(int *value, std::string &bits) {
	  	int sucss;
	    sucss = coder.decode_int(value, bits);

	    if(sucss != 0){
	    	std::cerr << "Sample decoding could not be performed!" << std::endl;
	    }
	};


public:
	Decoder (){

	}

	// inFile é o ficheiro codificado - outFile é o ficheiro descodificado
	void decode_Encoded_audio_file(std::string inFile, std::string outFile){

		// Abrir o ficheiro codificado
		BitStream bitStream = BitStream{inFile};
		// Vetor para armazenar os residuos
		std::vector<int> samples;
		// Ler cada um dos bits
		auto bit = bitStream.read_bit();
		// bits do q
		std::string q_bits;
		// bits do r
		std::string r_bits;
		//  Flag do q
		int flag_q = 1;
		// String do qr final
		std::string qr_bits;
		// Valor do residual
		int sample;
		int atual_residual;
		// Valor do last residual
		int last_residual = 0;

		// Criar o ficheiro para escrever o ficheiro de áudio
		SndfileHandle sfhOut{ outFile, SFM_WRITE, 65536, 2, 441000};


		while(bit != '-'){

			qr_bits = qr_bits  + std::to_string(bit);
			std::cout << bit;

			if(qr_bits.size() == 17){

				int sucss = coder.decode_int(&atual_residual, qr_bits);
				std::cout << qr_bits << std::endl;

				sample = atual_residual + last_residual;

				std::cout << atual_residual << std::endl;
				std::cout << sample << std::endl;

				last_residual = atual_residual;
				samples.push_back(sample);
				qr_bits.clear();
			}

			// Ler o próximo bit
			bit = bitStream.read_bit();
		}
	// Escrever no ficheiro .wav
	sfhOut.writef(samples.data(), 65536);
	}
};
#endif //P2_DECODER_H
