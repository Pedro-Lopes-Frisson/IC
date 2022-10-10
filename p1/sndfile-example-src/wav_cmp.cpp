#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_hist.h"
#include <cmath>

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames

int main(int argc, char *argv[]) {

	if(argc < 3) {
		cerr << "Usage: " << argv[0] << " <original file> <other file>\n";
		return 1;
	}

	// Abrir os dois ficheiros de audio; argc = 3
	SndfileHandle sndFile { argv[argc-2] }; // Original file
	SndfileHandle sndFile2 { argv[argc-1] }; // Other file

	// Implementação dos erros
	if(sndFile.error()) {
		cerr << "Error: invalid 1st input file\n";
		return 1;
    }
    if(sndFile2.error()) {
		cerr << "Error: invalid 2nd input file\n";
		return 1;
    }


    if((sndFile.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: 1st file is not in WAV format\n";
		return 1;
	}
	if((sndFile2.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: 2nd file is not in WAV format\n";
		return 1;
	}


	if((sndFile.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: 1st file is not in PCM_16 format\n";
		return 1;
	}
	if((sndFile2.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: 2nd file is not in PCM_16 format\n";
		return 1;
	}


	// Definir variáveis para obter as samples de cada file
	size_t nFrames;
	vector<short> samples(FRAMES_BUFFER_SIZE * sndFile.channels());

	size_t nFrames2;
	vector<short> samples2(FRAMES_BUFFER_SIZE * sndFile2.channels());

	// While loop que percorre todas as samples
	while((nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE)) && (nFrames2 = sndFile2.readf(samples2.data(), FRAMES_BUFFER_SIZE)) ) {

		samples.resize(nFrames * sndFile.channels());
		samples2.resize(nFrames2 * sndFile2.channels());

		// Percorrer cada sample
		for(int i = 0; i < samples.size(); i++){

			// Calculo do power signal
			float sample_powerSignal = pow(samples[i], 2);
			// Calculo do power noise
			float sample_powerNoise = pow((samples[i] - samples2[i]), 2);
			// Calculo do Signal noise ratio per sample
			float SNRperSample = (sample_powerSignal / sample_powerNoise);
			// Calculo do max absolute error per sample: valor obtido - valor esperado
			// É 1 porque se o noise for zero, samples2[i], o SNR fica = 1
			float maxABSerror = SNRperSample - 1;

			cout << "Sample " << samples[i] << ": SNR = " << SNRperSample << "\nMax Absolute Error = " << maxABSerror << '\n';
			cout << '\n';
		}

	}

	return 0;

}

// RUN: ../sndfile-example-bin/wav_cmp sample.wav quant.wav 