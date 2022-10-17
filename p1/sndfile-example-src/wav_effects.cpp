#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_hist.h"
#include <cmath>
#include <cstring>
#include <fstream>


using namespace std;

// Vamos criar uma macro que aplica echo a um ficheiro de audio wav
void  wav_echo2_16(fstream& filin, fstream& fileout, int eco_rate, double alfa){
	short int temp;
	short int ch[eco_rate], ch2[eco_rate];
	int i;
	for(i=0; i<eco_rate; i++){

	   ch2[i] = 0;

	}
	while(!filin.eof())
	{
	    for(i=0; i<eco_rate && !filin.eof(); i++){

	    	filin.read((char*)&ch[i], sizeof(short int));

	    }
	    for(i=0; i<eco_rate; i++){
	        temp  = ch[i];
	        ch[i]= ch[i] + alfa * ch2[i];
	        fileout.write((char*)&ch[i], sizeof(short int));
	        ch2[i] = temp;
	    }

	}
}
// O primeiro parametro de entrada é o ficheiro original, o segundo
//é o ficheiro onde vai estar o echo e o terceiro parametro é o rate do echo.

void wav_ampRecude(ifstream& wav_file, ofstream& out_file){
	wav_file.open("sample.wav",ios::binary|ios::ate);
	size_t file_size = wav_file.tellg();
	char * wav_buf = new char[file_size];
	wav_file.seekg (0,ios::beg);
	wav_file.read (wav_buf, file_size);
	wav_file.close();
	int16_t wav_smpl(0);
	char * wav_out = new char[file_size];
	memcpy(wav_out, wav_buf, 44);
	for (size_t i = 0 ; i < file_size ; i += 2)
	{
	    memcpy(&wav_smpl , wav_buf + (i + 44) , 2);
	    wav_smpl *= 0.5;
	    memcpy(wav_out + (i + 44) , &wav_smpl , 2);
	}
	out_file.open("sample_ampReduce.wav",ios::binary);
	out_file.write(wav_out, file_size);
	out_file.close();
}


int main() {

	// Introduzir echo
	fstream filin("/home/andre/Desktop/4º ANO/IC/Projeto1/IC/p1/sndfile-example-src/sample.wav", ios::in|ios::binary);
	fstream fileout_echo("/home/andre/Desktop/4º ANO/IC/Projeto1/IC/p1/sndfile-example-src/sample_echo.wav",ios::out| ios::binary);

	wav_echo2_16(filin, fileout_echo, 14000, 0.8);


	// Reduzir amplitude
	ifstream wav_file;
    ofstream out_file;

    wav_ampRecude(wav_file, out_file);

	return 0;

}

// https://stackoverflow.com/questions/23745811/adding-echo-effect-to-a-wav-file
// https://stackoverflow.com/questions/57709983/distorted-output-during-processing-wav-file
