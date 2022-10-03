#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_quant.h"

using namespace std;
constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames

int main(int argc, char *argv[]) {

	SndfileHandle sndFile { argv[argc-1] };

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

	WAVQuant qnt { 8 };

	SndfileHandle sfhOut { "quant.wav", SFM_WRITE, sndFile.format(),
	  sndFile.channels(), sndFile.samplerate() };

	while((nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) { // 10 2
		samples.resize(nFrames * sndFile.channels());
		qnt.apply_quantitization(samples);
		sfhOut.writef(samples.data(), nFrames);
	}


  return 0;
}
