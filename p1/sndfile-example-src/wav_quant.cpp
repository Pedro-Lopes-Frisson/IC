#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_quant.h"

using namespace std;
constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames

int main(int argc, char *argv[]) {


    if(argc != 3){
        cerr<< "Usage ./wav_quant <input-file> <output-file> where output file is a quantized version of input files";
    }

	SndfileHandle sndFile { argv[argc-3] };
	if(sndFile.error()) {
		cerr << "Error: invalid input file\n";
		return 1;
    }

	if((sndFile.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: file is not in WAV format\n";
		return 1;
	}

	if((sndFile.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: file is not in PCM_16 format\n";
		return 1;
	}

	size_t nFrames;
	vector<short> samples(FRAMES_BUFFER_SIZE * sndFile.channels());

	WAVQuant qnt {  atoi(argv[argc - 1])};

	SndfileHandle sfhOut { argv[argc - 2], SFM_WRITE, sndFile.format(),
	  sndFile.channels(), sndFile.samplerate() };

	while((nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
		samples.resize(nFrames * sndFile.channels());
		qnt.apply_quantitization(samples);
		sfhOut.writef(samples.data(), nFrames);
	}


  return 0;
}
