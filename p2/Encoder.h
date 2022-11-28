#ifndef P2_ENCODER_H
#define P2_ENCODER_H

#include "BitStream.h"
#include "GolombCoding.h"
#include <string>
#include <sndfile.hh>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <cmath>


class Encoder {
private:
  const static long FRAMES_BUFFER_SIZE = 65536;
  
  std::vector<short> lastvalues;
  std::vector<short> lastvaluesSide;
  std::vector<short> block_prev_mid;
  std::vector<short> block_prev_side;
  GolombCoder coder_mid{10};
  GolombCoder coder_side{10};
  int block_samples_mid = 0;
  int block_samples_side = 0;
  
  int open_wav(SndfileHandle &inFile, std::string filename) {
    inFile = SndfileHandle(filename);
    if (inFile.error()) {
      std::cerr << "Error: invalid input file\n";
      return 1;
    }
    
    if ((inFile.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
      std::cerr << "Error: file is not in WAV format\n";
      return 1;
    }
    
    if ((inFile.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
      std::cerr << "Error: file is not in PCM_16 format\n";
      return 1;
    }
    return 0;
  };
  
  //int encode_residual(short value, std::string &bits) {
  void encode_residual(short value, std::string &bits, int mid) {
    if (mid)
      coder_mid.encode_int(value, bits);
    else
      coder_side.encode_int(value, bits);
  };
  
  void write_to_file(std::string bits, BitStream &bitStream) {
    for (size_t i = 0; i < bits.size(); i++) {
      bitStream.writeBit(bits[i] == '1');
    }
  }
  
  int predict(int mid) {
    if (mid) {
      return lastvalues[0] - 3 * lastvalues[1] + 3 * lastvalues[2];
    } else {
      return lastvaluesSide[0] - 3 * lastvaluesSide[1] + 3 * lastvaluesSide[2];
    }
    
  }
  
  void add_new_sample(short xn_1, int mid) {
    if (mid) {
      lastvalues.erase(lastvalues.begin()); // remove the oldest predictor
      lastvalues.push_back(xn_1);
    } else {
      lastvaluesSide.erase(lastvaluesSide.begin()); // remove the oldest predictor
      lastvaluesSide.push_back(xn_1);
      
    }
  }
  
  void add_new_sample_block_mid(short xn_1) {
    // increment first that way we check if
    // blocksize values have been written and update M accordingly
    increment_block_samples_counter(1);
    block_prev_mid.erase(block_prev_mid.begin()); // remove the oldest predictor
    block_prev_mid.push_back(xn_1);
  }
  
  void add_new_sample_block_side(short xn_1) {
    // increment first that way we check if
    // blocksize values have been written and update M accordingly
    increment_block_samples_counter(0);
    block_prev_side.erase(block_prev_side.begin()); // remove the oldest predictor
    block_prev_side.push_back(xn_1);
  }
  
  void increment_block_samples_counter(int mid) {
    if (mid) {
      if (block_samples_mid == block_prev_mid.size() - 1) {
        // block samples was filled
        // update M
        calculate_new_m(mid);
        
      }
      block_samples_mid = (block_samples_mid + 1) % block_prev_mid.size(); // 0 a 1023
      return;
    }
    
    if (block_samples_side == block_prev_side.size() - 1) {
      // block samples was filled
      // update M
      calculate_new_m(mid);
      
    }
    block_samples_side = (block_samples_side + 1) % block_prev_side.size(); // 0 a 1023
  }
  
  void calculate_new_m(int mid) {
    if (mid) {
      double sample_mean = 0;
      for (auto c: block_prev_mid) {
        if (c < 0)
          sample_mean += 2 * abs(c) + 1;
        else
          sample_mean += 2 * c;
      }
      sample_mean /= block_prev_mid.size();
      
      
      // calculate M
      double m = 0;
      
      // round M to the nearest power of 2
      std::cout << std::endl << "Mid, Previous M: " << coder_mid.M;
      if (sample_mean > 0)
        m = ceil(log2(sample_mean / 2));
      
      coder_mid.M = pow(2, m);
      std::cout << std::endl << "MID, Next M: " << coder_mid.M;
      return;
    }
    double sample_mean = 0;
    for (auto c: block_prev_side) {
      if (c < 0)
        sample_mean += 2 * abs(c) + 1;
      else
        sample_mean += 2 * c;
    }
    sample_mean /= block_prev_side.size();
    
    
    // calculate M
    double m = 0;
    
    // round M to the nearest power of 2
    std::cout << std::endl << "side, Previous M: " << coder_side.M;
    if (sample_mean > 0)
      m = ceil(log2(sample_mean / 2));
    
    coder_side.M = pow(2, m);
    std::cout << std::endl << "Side, Next M: " << coder_side.M;
  }


public:
  Encoder(int k, int block) {
    coder_side.M = pow(2, k);
    coder_mid.M = pow(2, k);
    block_prev_side.resize(block);
    block_prev_mid.resize(block);
    this->block_samples_mid = 0;
    this->block_samples_side = 0;
    lastvalues.resize(3);
    lastvaluesSide.resize(3);
    
    std::fill(lastvalues.begin(), lastvalues.end(), 0);
    std::fill(lastvaluesSide.begin(), lastvaluesSide.end(), 0);
    std::fill(block_prev_mid.begin(), block_prev_mid.end(), 0);
    std::fill(block_prev_side.begin(), block_prev_side.end(), 0);
  }
  
  void encode_audio_file(std::string inFile, std::string outFile) {
    // abrir audio file
    SndfileHandle sndFile;
    BitStream bitStream = BitStream(outFile.data(), BT_WRITE);
    int res = open_wav(sndFile, inFile);
    if (res != 0) {
      std::cerr << "File could not be open" << std::endl;
    }
    // Definir variaveis
    size_t nFrames;
    std::vector<short> samples(FRAMES_BUFFER_SIZE * sndFile.channels());
    short residual;
    //short lastValue = 0;
    std::string bits;
    std::ofstream f1{"res_bits", std::ofstream::app};
    std::ofstream f2{"side_bits", std::ofstream::app};
    
    // Percorrer as samples todas
    long long int i = 0;
    /*
    encode_residual(sndFile.samplerate(), bits);
    write_to_file(bits, bitStream);
    
    encode_residual(sndFile.channels(), bits);
    write_to_file(bits, bitStream);
     */
    short left_sample, right_sample;
    long long int mid, side;
    long long int c = 0;
    int l;
    int r;
    int l_r;
    
    if (sndFile.channels() != 2) {
      std::cerr << "ERRRRRRRRRO";
      std::exit(-1);
    }
    // TODO: Write Sample Rate and Number of channels
    
    while ((nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) { // 10 2
      samples.resize(nFrames * sndFile.channels());
      for (l = 0, r = 1; l < samples.size() - 1 && r < samples.size(); r += 2, l += 2) {
        c++;
        left_sample = samples[l];
        right_sample = samples[r];
        l_r = left_sample + right_sample;
        
        
        if (l_r % 2 == 1) {
          l_r = 2 * l_r + 1;
        }
        mid = l_r / 2;
        side = left_sample - right_sample;
        
        
        residual = mid - predict(1);
        // gets Golomb Code for the prediction residual
        encode_residual(residual, bits, 1);
        write_to_file(bits, bitStream);
        std::cout << "MID: " << mid << std::endl;
        std::cout << "MID Bits: " << bits << std::endl;
        bits.clear();
        
        short residual_side = side - predict(0);
        encode_residual(residual_side, bits, 0);
        write_to_file(bits, bitStream);
        std::cout << "SIDE: " << side << std::endl;
        std::cout << "SIDE Bits: " << bits << std::endl;
        
        // add new sample to improve Golomb M
        add_new_sample_block_mid(residual);
        add_new_sample_block_side(residual_side);
        
        // Adds a new sample to use in the predictor
        add_new_sample(mid, 1);
        add_new_sample(side, 0);
      }
      
    }
    
    
    sndFile.writef(samples.data(), samples.size() / sndFile.channels());
    
    bitStream.close();
    f1.close();
    f2.close();
  }
};


#endif //P2_ENCODER_H