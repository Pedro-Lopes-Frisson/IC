#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_hist.h"
#include <cmath>
#include <bits/stdc++.h>

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

	// Potência do sinal é a soma de cada valor lido ao quadrado
	double signal_power = 0;
	// Potência do sinal com ruído é a soma da diferença entre as amostras dos dois ficheiros ao quadrado
	double noise_signal_power = 0;
	// Vetor que armazena os erros absolutos das samples
	vector<double> sample_abs_error;
	// While loop que percorre todas as samples
	while((nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE)) && (nFrames2 = sndFile2.readf(samples2.data(), FRAMES_BUFFER_SIZE)) ) {

		samples.resize(nFrames * sndFile.channels());
		samples2.resize(nFrames2 * sndFile2.channels());

		// Percorrer cada sample
		for(int i = 0; i < samples.size(); i++){

			// Calculo do power signal
			float sample_powerSignal = pow(samples[i], 2);
			// Calculo do power signal com noise
			float sample_powerNoise = pow(samples[i] - samples2[i], 2);

			signal_power += sample_powerSignal;
			noise_signal_power += sample_powerNoise;

			double AbsErr = samples[i] - samples2[i];
			sample_abs_error.push_back(AbsErr);

		}


	}

	// Calculo do SNR de cada sample
	double SNR = 10 * log(signal_power / noise_signal_power);
	cout << "SNR = " << SNR << "\nMax Abs. Error = " << *max_element(sample_abs_error.begin(), sample_abs_error.end()) << '\n';

	return 0;

}

// RUN: ../sndfile-example-bin/wav_cmp sample.wav quant.wav
