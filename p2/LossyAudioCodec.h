//
// Created by whoknows on 11/30/22.
//

#ifndef IC_LOSSYAUDIOCODEC_H
#define IC_LOSSYAUDIOCODEC_H

#include "BitStream.h"
#include "GolombCoding.h"
#include <string>
#include <sndfile.hh>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <cmath>
#include <bitset>
#include <fftw3.h>


class LossyAudioCodec {
  /*
   * Lossy audio codec with DCT coefficients encoded with Golomb
   */

private:
  GolombCoder encoder = GolombCoder(10);
  int dct_bs; // dct blocksize
  size_t block_samples_cnt; // count of number of samples to store to recalculate Golomb Parameter
  std::vector<int> block_samples;
  double dctFrac; // fraction of frequencies to remove
  
  void add_sample_to_block(int sample) {
    increment_block_cnt();
    block_samples.erase(block_samples.begin());
    block_samples.push_back(sample);
  }
  
  void increment_block_cnt() {
    if (block_samples_cnt + 1 >= block_samples.size()) {
      calculate_new_m();
    }
    block_samples_cnt = (block_samples_cnt + 1) % block_samples.size();
  }
  
  void calculate_new_m() {
    double sample_mean = 0;
    for (auto c: block_samples) {
      if (c < 0)
        sample_mean += 2 * abs(c) + 1;
      else
        sample_mean += 2 * c;
    }
    sample_mean /= block_samples.size();
    
    // calculate M
    double m = 0;
    
    // round M to the nearest power of 2
    if (sample_mean > 0)
      m = ceil(log2(sample_mean / 2));
    
    encoder.M = pow(2, m);
    if (m <= 0)
      encoder.M = 1;
  }
  
  int open_wav_read(SndfileHandle &file, std::string filename) {
    file = SndfileHandle(filename);
    if (file.error()) {
      std::cerr << "Error: invalid input file\n";
      return 1;
    }
    
    if ((file.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
      std::cerr << "Error: file is not in WAV format\n";
      return 1;
    }
    
    if ((file.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
      std::cerr << "Error: file is not in PCM_16 format\n";
      return 1;
    }
    return 0;
  };
  
  int open_wav_write(SndfileHandle &file, std::string filename, int channels, long long int srate) {
    file = SndfileHandle(filename.data(), SFM_WRITE, 65538, channels, srate);
    
    if (file.error()) {
      std::cerr << "Error: invalid input file\n";
      return 1;
    }
    
    if ((file.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
      std::cerr << "Error: file is not in WAV format\n";
      return 1;
    }
    
    if ((file.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
      std::cerr << "Error: file is not in PCM_16 format\n";
      return 1;
    }
    
    return 0;
  };
  
  void write_to_file(std::string bits, BitStream &bitStream) {
    for (size_t i = 0; i < bits.size(); i++) {
      bitStream.writeBit(bits[i] == '1');
    }
  }
  
  void reset_variables() {
    encoder.M = 1;
    block_samples_cnt = 0;
    
    std::fill(block_samples.begin(), block_samples.end(), 0);
  }

public:
  LossyAudioCodec(int k, int b, long int dct_bs, double frac) {
    encoder.M = pow(2, k);
    block_samples_cnt = 0;
    block_samples.resize(b);
    this->dct_bs = dct_bs;
    dctFrac = frac;
    
    std::fill(block_samples.begin(), block_samples.end(), 0);
  }
  
  /*
   * encode inFile to outfile
   * */
  void encode(std::string inFile, std::string outFile) {
    // Read audio file
    SndfileHandle f;
    int res = open_wav_read(f, inFile);
    if (res != 0) {
      std::cerr << "Error audio file not open!.";
      return;
    }
    
    size_t nChannels{static_cast<size_t>(f.channels())};
    size_t nFrames{static_cast<size_t>(f.frames())};
    
    
    std::vector<short> samples(nChannels * nFrames);
    f.readf(samples.data(), nFrames);
    
    size_t nBlocks{static_cast<size_t>(ceil(static_cast<double>(nFrames) / dct_bs))};
    
    // Do zero padding, if necessary
    samples.resize(nBlocks * dct_bs * nChannels);
    
    // Vector for holding all DCT coefficients, channel by channel
    std::vector <std::vector<double>> x_dct(nChannels, std::vector<double>(nBlocks * dct_bs));
    
    // Vector for holding DCT computations
    std::vector<double> x(dct_bs);
    BitStream bitStream = BitStream(outFile.data(), BT_WRITE);
    
    std::string s = std::bitset<16>(f.samplerate()).to_string(); // string conversion
    // write SampleRate
    for (size_t h = 0; h < s.size(); h++) {
      bitStream.writeBit('1' == s[h]);
    }
    // write Number of Channels
    s = std::bitset<8>(f.channels()).to_string(); // string conversion
    for (size_t h = 0; h < s.size(); h++) {
      bitStream.writeBit('1' == s[h]);
    }
    // write Golomb Parameter its the same for both channels
    s = std::bitset<8>(log2(encoder.M)).to_string(); // string conversion
    for (size_t h = 0; h < s.size(); h++) {
      bitStream.writeBit('1' == s[h]);
    }
    // write Block size its the same for both channels
    s = std::bitset<16>(block_samples.size()).to_string(); // string conversion
    for (size_t h = 0; h < s.size(); h++) {
      bitStream.writeBit('1' == s[h]);
    }
    
    // write dct Block size its the same for both channels
    s = std::bitset<16>(dct_bs).to_string(); // string conversion
    for (size_t h = 0; h < s.size(); h++) {
      bitStream.writeBit('1' == s[h]);
    }
    
    // write dctFrac
    int dct_frac_int = static_cast<int> (dctFrac * 100);
    s = std::bitset<8>(dct_frac_int).to_string(); // string conversion
    std::cout << "S: " << s << std::endl;
    for (size_t h = 0; h < s.size(); h++) {
      bitStream.writeBit('1' == s[h]);
    }
    // write Number of Blocks
    s = std::bitset<16>(nBlocks).to_string(); // string conversion
    for (size_t h = 0; h < s.size(); h++) {
      bitStream.writeBit('1' == s[h]);
    }
    
    // Direct DCT
    fftw_plan plan_d = fftw_plan_r2r_1d(dct_bs, x.data(), x.data(), FFTW_REDFT10, FFTW_ESTIMATE);
    for (size_t n = 0; n < nBlocks; n++) {
      for (size_t c = 0; c < nChannels; c++) {
        for (size_t k = 0; k < dct_bs; k++) {
          x[k] = samples[(n * dct_bs + k) * nChannels + c];
        }
        fftw_execute(plan_d);
        // Keep only "dctFrac" of the "low frequency" coefficients
        for (size_t k = 0; k < dct_bs * dctFrac; k++) {
          x_dct[c][n * dct_bs + k] = x[k] / (dct_bs << 1);
        }
      }
    }
    
    // write DCT Coefficients
    std::string bits;
    for (size_t n = 0; n < nBlocks; n++) {
      for (size_t c = 0; c < nChannels; c++) {
        // Keep only "dctFrac" of the "low frequency" coefficients
        for (size_t k = 0; k < dct_bs * dctFrac; k++) {
          long long int value = (long long int) round(x_dct[c][n * dct_bs + k]);
          encoder.encode_int(value, bits);
          std::cout << bits << std::endl;
          std::cout << value << std::endl;
          write_to_file(bits, bitStream);
          
          add_sample_to_block(value);
          
        }
      }
    }
    bitStream.close();
    
  };
  
  void decode_audio_file(std::string encodedFile, std::string wav_file) {
    BitStream bitStream = BitStream(encodedFile.data(), BT_READ);
    
    SndfileHandle f;
    long int sampleRate = 0;
    int nChannels = 0;
    
    // read samplerate
    int bits_sR[16];
    bitStream.getNBit(bits_sR, 16);
    for (int h = 0; h < 16; h++) {
      if (bits_sR[h])
        sampleRate += pow(2, 15 - h);
    }
    
    // read number of channels
    int bits_nChannels[8];
    bitStream.getNBit(bits_nChannels, 8);
    
    for (int h = 0; h < 8; h++) {
      if (bits_nChannels[h])
        nChannels += pow(2, 7 - h);
    }
    
    
    // Decode Golomb Parameter it's the same for both coder's
    bitStream.getNBit(bits_nChannels, 8);
    encoder.M = 0;
    for (int h = 0; h < 8; h++) {
      if (bits_nChannels[h]) // get initial M
        encoder.M += pow(2, 7 - h);
    }
    encoder.M = pow(2, encoder.M);
    block_samples_cnt = 0;
    
    // Decode Block Size
    int bs = 0;
    // read block size
    bitStream.getNBit(bits_sR, 16);
    for (int h = 0; h < 16; h++) {
      if (bits_sR[h])
        bs += pow(2, 15 - h);
    }
    block_samples.resize(bs);
    
    // Decode DCT Block Size
    bs = 0;
    // read dct block size
    bitStream.getNBit(bits_sR, 16);
    for (int h = 0; h < 16; h++) {
      if (bits_sR[h])
        bs += pow(2, 15 - h);
    }
    dct_bs = bs;
    
    // Decode DCT frac
    bs = 0;
    // read dct FRAC
    bitStream.getNBit(bits_nChannels, 8);
    for (int h = 0; h < 8; h++) {
      std::cout << bits_nChannels[h];
      if (bits_nChannels[h]) {
        bs += pow(2, 7 - h);
      }
    }
    dctFrac = static_cast<double>(bs) / 100;
    std::cout << std::endl;
    
    size_t nBlocks = 0;
    // read dct block size
    bitStream.getNBit(bits_sR, 16);
    for (int h = 0; h < 16; h++) {
      if (bits_sR[h])
        nBlocks += pow(2, 15 - h);
    }
    
    int res = open_wav_write(f, wav_file, nChannels, sampleRate);
    if (res != 0){
      std::cerr << "Error" << std::endl;
    }
    
    size_t nFrames = ceil(nBlocks * (double) dct_bs);
    
    int bit;
    bool eof_flag = 0;
    const char one_num = '1';
    const char zero_num = '0';
    std::string q_r;
    std::vector<int> remainder_bits;
    
    std::vector<short> samples(nChannels * nFrames);
    std::vector <std::vector<double>> x_dct(nChannels, std::vector<double>(nBlocks * dct_bs));
    std::vector<double> x(dct_bs);
    long long int decoded_value;
    
    std::cout << "nChannels " << nChannels << std::endl;
    std::cout << "nBlocks " << nBlocks << std::endl;
    std::cout << "dct_bs " << dct_bs << std::endl;
    std::cout << "dct_frac " << dctFrac << std::endl;
    
    for (size_t n = 0; n < nBlocks; n++) {
      for (size_t c = 0; c < nChannels; c++) {
        for (size_t k = 0; k < dct_bs * dctFrac; k++) {
          bit = bitStream.getBit();
          // read q
          while (true) { // break only if unary code is finished
            if (bit == EOF) {
              eof_flag = 1;
              break;
            }
            if (bit == 0) {
              // unary code has finished
              q_r += zero_num;
              break;
            } else if (bit == 1) {
              q_r += one_num;
            }
            bit = bitStream.getBit();
          }
          
          // read remainder
          int read_bits;
          read_bits = log2(encoder.M);
          
          remainder_bits.resize(read_bits);
          
          bitStream.getNBit(remainder_bits.data(), read_bits);
          // create binary string
          for (int j = 0; j < read_bits; j++) {
            if (remainder_bits[j] == 1) {
              q_r += one_num;
            } else {
              q_r += zero_num;
            }
          }
          if (eof_flag == 1) {
            return;
          }
          
          encoder.decode_int(&decoded_value, q_r);
          std::cout << q_r << std::endl;
          std::cout << decoded_value << std::endl;
          q_r.clear();
          add_sample_to_block(decoded_value);
          x_dct[c][n * dct_bs + k] = decoded_value;
        }
      }
    }
    
    // Inverse DCT
    fftw_plan plan_i = fftw_plan_r2r_1d(dct_bs, x.data(), x.data(), FFTW_REDFT01, FFTW_ESTIMATE);
    for (size_t n = 0; n < nBlocks; n++)
      for (int c = 0; c < nChannels; c++) {
        for (size_t k = 0; k < dct_bs; k++) {
          x[k] = x_dct[c][n * dct_bs + k];
        }
        fftw_execute(plan_i);
        for (size_t k = 0; k < dct_bs; k++) {
          samples[(n * dct_bs + k) * nChannels + c] = static_cast<short>(round(x[k]));
        }
      }
    
    f.writef(samples.data(), samples.size() / nChannels);
    bitStream.close();
    
  };
  
  
};

#endif //P2_DECODER_H
