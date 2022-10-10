#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_hist.h"

using namespace std;

constexpr size_t
FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames

int main(int argc, char *argv[]) {

    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <input file> <channel>\n";
        return 1;
    }

    SndfileHandle sndFile{argv[argc - 1]};
    if (sndFile.error()) {
        cerr << "Error: invalid input file\n";
        return 1;
    }

    if ((sndFile.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
        cerr << "Error: file is not in WAV format\n";
        return 1;
    }

    if ((sndFile.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
        cerr << "Error: file is not in PCM_16 format\n";
        return 1;
    }

    int channel{stoi(argv[argc - 1])};
    if (channel >= sndFile.channels()) {
        cerr << "Error: invalid channel requested\n";
        return 1;
    }

    size_t nFrames;
    vector<short> samples(FRAMES_BUFFER_SIZE * sndFile.channels());

    WAVHist hist{sndFile};

    ofstream myfile;
    myfile.open("_quant_MID.txt");

    ofstream myfile1;
    myfile1.open("_quant_SIDE.txt");

    while ((nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) { // 10 2
        samples.resize(nFrames * sndFile.channels());

        hist.update(samples);
        hist.update_MID(samples);
        hist.update_SIDE(samples);
    }

    hist.dump(channel);
    hist.dump_MID(myfile);
    hist.dump_SIDE(myfile1);
    myfile.close();
    myfile1.close();

    return 0;
}
