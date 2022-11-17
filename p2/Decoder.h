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
		std::vector<int> residuos;
		// Ler cada um dos bits
		auto bit = bitStream.read_bit();
		// Percorrer todos os bits do ficheiro
		std::string k_bits;
		// Bandeira que simboliza que o qr_bits ainda nao esta cheio
		int flag = 0;
		// Quociente do M ideal é sempre 0, entao é o bit logo asseguir ao k
		// String para o  q + r
		std::string qr_bits;
		// Inicializar o k
		int k;

		while (bit != '-'){
			std::cout << bit << std::endl;
			// ENCONTRAR O K
			if (bit != '0' && flag == 0){
				k_bits = k_bits + std::to_string(bit);
			}
			// Já lemos o K
			else{
				if (flag == 0){
					// Metemos o k a inteiro e damos reset à k_bits
					k = stoi(k_bits);
					std::string k_bits;
					// Calculamos o M
					//int M = pow(2, k);
					// Adicionamos o bit do quociente
					qr_bits = qr_bits + std::to_string(bit);
					flag = 1;
				}
				else { 
					qr_bits = qr_bits + std::to_string(bit);
					// Já temos o r todo
					if(qr_bits.size() == k){
						// Damos reset à flag
						flag = 0;
						// Criar o inteiro que nos vai dar o residual descodificado
						int residual;
						int sucss = coder.decode_int( &residual, qr_bits);
						// Reset ao array
						std::string qr_bits;
						//Reset ao k
						int k;
						// Escrever no vetor dos residuos
						residuos.push_back(residual);
						std::cout << residuos.data() << std::endl;
					}
				}
			}

		}
		// Escrever no ficheiro .wav
		//SndfileHandle sfhOut{outFile};
		//sfhOut.writef(residuos.data());
	}


};

#endif //P2_DECODER_H