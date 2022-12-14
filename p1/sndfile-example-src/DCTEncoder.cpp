//
// Created by whoknows on 10/14/22.
//

#include <iostream>
#include <vector>
#include <cmath>
#include <fftw3.h>
#include <sndfile.hh>
#include "bitStream.h"

using namespace std;


short int quantize_value(double *value);

int main(int argc, char *argv[]) {

    bool verbose{false};
    size_t bs{1024};
    double dctFrac{0.2};

    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " [ -v (verbose) ]\n";
        cerr << "               [ -bs blockSize (def 1024) ]\n";
        cerr << "               [ -frac dctFraction (def 0.2) ]\n";
        cerr << "               wavFileIn \n";
        cerr << "               encoded file out \n";
        return 1;
    }

    for (int n = 1; n < argc; n++)
        if (string(argv[n]) == "-v") {
            verbose = true;
            break;
        }

    for (int n = 1; n < argc; n++)
        if (string(argv[n]) == "-bs") {
            bs = atoi(argv[n + 1]);
            break;
        }

    for (int n = 1; n < argc; n++)
        if (string(argv[n]) == "-frac") {
            dctFrac = atof(argv[n + 1]);
            break;
        }

    SndfileHandle sfhIn{argv[argc - 2]};
    if (sfhIn.error()) {
        cerr << "Error: invalid input file\n";
        return 1;
    }

    if ((sfhIn.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
        cerr << "Error: file is not in WAV format\n";
        return 1;
    }

    if ((sfhIn.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
        cerr << "Error: file is not in PCM_16 format\n";
        return 1;
    }

    if (verbose) {
        cout << "Input file has:\n";
        cout << '\t' << sfhIn.frames() << " frames\n";
        cout << '\t' << sfhIn.samplerate() << " samples per second\n";
        cout << '\t' << sfhIn.channels() << " channels\n";
    }

    size_t nChannels{static_cast<size_t>(sfhIn.channels())};
    size_t nFrames{static_cast<size_t>(sfhIn.frames())};


    // Read all samples: c1 c2 ... cn c1 c2 ... cn ...
    // Note: A frame is a group c1 c2 ... cn
    vector<short> samples(nChannels * nFrames);
    sfhIn.readf(samples.data(), nFrames);

    size_t nBlocks{static_cast<size_t>(ceil(static_cast<double>(nFrames) / bs))};

    // Do zero padding, if necessary
    samples.resize(nBlocks * bs * nChannels);

    // Vector for holding all DCT coefficients, channel by channel
    vector <vector<double>> x_dct(nChannels, vector<double>(nBlocks * bs));

    // Vector for holding DCT computations

    vector<double> x(bs);
    long int written_coefficients = 0;

    // Direct DCT
    fftw_plan plan_d = fftw_plan_r2r_1d(bs, x.data(), x.data(), FFTW_REDFT10, FFTW_ESTIMATE);
    for (size_t n = 0; n < nBlocks; n++)
        for (size_t c = 0; c < nChannels; c++) {
            for (size_t k = 0; k < bs; k++)
                x[k] = samples[(n * bs + k) * nChannels + c];

            fftw_execute(plan_d);
            // Keep only "dctFrac" of the "low frequency" coefficients
            for (size_t k = 0; k < bs * dctFrac; k++) {
                x_dct[c][n * bs + k] = x[k] / (bs << 1);
            }

        }
    cout << "[";
    for (auto val : x_dct[0]){
        cout << val << ",";
    }
    cout << "]" << endl << endl;

    BitStream bitStream{argv[argc - 1]};
    bitset<16> coefficients_encoded;
    int value;
    bitset<24> frames_num = bitset<24>(nFrames);
    bitStream.write_Nbit(frames_num);
  /*  for (int j = 16; j < 24; j++) {
        bitStream.write_bit(frames_num[j]);
    }
    for (int j = 8; j < 16; j++) {
        bitStream.write_bit(frames_num[j]);
    }
    for (int j = 0; j < 8; j++) {
        bitStream.write_bit(frames_num[j]);
    }*/


    //write number of channels
    bitset<16> channel_num = bitset<16>(nChannels);
    bitStream.write_Nbit(channel_num);
//    for (int j = 8; j < 16; j++) {
//        bitStream.write_bit(channel_num[j]);
//    }
//    for (int j = 0; j < 8; j++) {
//        bitStream.write_bit(channel_num[j]);
//    }


    //write number of block size
    bitset<16> bs_num = bitset<16>(bs);
    bitStream.write_Nbit(bs_num);
/*    for (int j = 8; j < 16; j++) {
        bitStream.write_bit(bs_num[j]);
    }
    for (int j = 0; j < 8; j++) {
        bitStream.write_bit(bs_num[j]);
    }*/

    cout << "NFrames: " << nFrames << endl;
    cout << "NChannels: " << nChannels << endl;
    cout << "bs: " << bs << endl;
    cout << "SR: " << sfhIn.samplerate() << endl;
    cout << "W C: " << dctFrac << endl;

    //write samplerate
    bitset<16> bs_sr = bitset<16>(sfhIn.samplerate());
    bitStream.write_Nbit(bs_sr);
/*
    for (int j = 8; j < 16; j++) {
        bitStream.write_bit(bs_sr[j]);
    }
    for (int j = 0; j < 8; j++) {
        bitStream.write_bit(bs_sr[j]);
    }
*/

    bitset<16> dct_frac_bits = bitset<16>(round(dctFrac * 100));
    cout << "DCT Bits : " << dct_frac_bits << endl;
    bitStream.write_Nbit(dct_frac_bits);
/*
    for (int j = 8; j < 16; j++) {
        bitStream.write_bit(dct_frac_bits[j]);
    }
    for (int j = 0; j < 8; j++) {
        bitStream.write_bit(dct_frac_bits[j]);
    }
*/

    for (size_t n = 0; n < nBlocks; n++)
        for (size_t c = 0; c < nChannels; c++) {
            // Keep only "dctFrac" of the "low frequency" coefficients
            for (size_t k = 0; k < bs * dctFrac; k++) {
                auto value = round(x_dct[c][n * bs + k]);
                coefficients_encoded = bitset<16>(value);
                for (int k = 8; k < 16; k++) {
                    bitStream.write_bit(coefficients_encoded[k]);
                }
                for (int k = 0; k < 8; k++) {
                    bitStream.write_bit(coefficients_encoded[k]);
                }
            }

        }



    bitStream.close_files();
    return 0;
}
