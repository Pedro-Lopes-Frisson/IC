//
// Created by whoknows on 10/16/22.
//


#include <iostream>
#include <vector>
#include <cmath>
#include <fftw3.h>
#include <sndfile.hh>
#include "bitStream.h"

using namespace std;


int main(int argc, char *argv[]) {

    int nChannels{2};
    size_t nFrames{1};
    size_t bs{1024};
    auto sr{441000};

    if (argc < 3) {
        cerr << "Usage: wav_dct number_of_channels\n";
        cerr << "                        coded_wav\n";
        cerr << "                        wavOut\n";
        return 1;
    }
    BitStream bitStream = BitStream{argv[argc - 2], "no_write"};
    bitset<16> bits = bitset<16>(0);
    bitset<24> bits_Frames = bitset<24>(0);

    for (int j = 0; j < 24; ++j) {
        bits_Frames[23 - j] = bitStream.read_bit();
    }
    nFrames = bits_Frames.to_ulong();
    for (int i = 0; i < 3; i++) {
        bits.reset();
        for (int j = 0; j < 16; ++j) {
            bits[15 - j] = bitStream.read_bit();
        }
        if (i == 0)
            nChannels = bits.to_ulong();
        if (i == 1)
            bs = bits.to_ulong();
        if (i == 2)
            sr = bits.to_ulong();
    }
    cout << "NFrames: " << nFrames << endl;
    cout << "NChannels: " << nChannels << endl;
    cout << "bs: " << bs << endl;
    cout << "SR: " << sr << endl;


    vector<short> samples(nChannels * nFrames);

    size_t nBlocks{static_cast<size_t>(ceil(static_cast<double>(nFrames) / bs))};

    // Do zero padding, if necessary
    samples.resize(nBlocks * bs * nChannels);

    // Vector for holding all DCT coefficients, channel by channel
    vector <vector<double>> x_dct(nChannels, vector<double>(nBlocks * bs));

    // Vector for holding DCT computations
    vector<double> x(bs);


    cout << endl;
    bitset<16> coefficients_decode;
    for (size_t i = 0; i < x_dct.size(); i++) {
        for (size_t j = 0; j < x_dct[i].size(); j++) {
            x_dct[i][j] = 0;
            for (int k = 0; k < 16; k++) {
                int bit = bitStream.read_bit();
                coefficients_decode.set(15 - k, bit);
            }
            x_dct[i][j] = (int16_t) coefficients_decode.to_ulong();
        }
    }
    SndfileHandle sfhOut{argv[argc - 1], SFM_WRITE, 65538,
                         nChannels, sr};

    cout << "FORMAT " << sfhOut.format() << endl;
    cout << "Channels " << sfhOut.channels() << endl;
    cout << "SR " << sfhOut.samplerate() << endl;
    // Inverse DCT
    fftw_plan plan_i = fftw_plan_r2r_1d(bs, x.data(), x.data(), FFTW_REDFT01, FFTW_ESTIMATE);
    for (size_t n = 0; n < nBlocks; n++)
        for (int c = 0; c < nChannels; c++) {
            for (size_t k = 0; k < bs; k++) {
                x[k] = x_dct[c][n * bs + k];
            }
            fftw_execute(plan_i);
            for (size_t k = 0; k < bs; k++)
                samples[(n * bs + k) * nChannels + c] = static_cast<short>(round(x[k]));

        }

    sfhOut.writef(samples.data(), nFrames);

    return 0;
}

