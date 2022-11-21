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
  std::vector<short> block_prev;
  GolombCoder coder{10};
  int block_samples = 0;
  
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
  void encode_residual(short value, std::string &bits) {
    coder.encode_int(value, bits);
  };
  
  void write_to_file(std::string bits, BitStream &bitStream) {
    for (size_t i = 0; i < bits.size(); i++) {
      bitStream.writeBit(bits[i] == '1');
    }
  }
  
  int predict() {
    return block_prev[0] - 3 * block_prev[1] + 3 * block_prev[2];
  }
  
  void add_new_sample(short xn_1) {
    block_prev.erase(block_prev.begin()); // remove the oldest predictor
    block_prev.push_back(xn_1);
  }
  // arr.pop(0)
  //arr.pushback new val
  // 0,0,0
  // 0,0,1
  
  void add_new_sample_block(short xn_1) {
    // increment first that way we check if
    // blocksize values have been written and update M accordingly
    increment_block_samples_counter();
    block_prev.erase(block_prev.begin()); // remove the oldest predictor
    block_prev.push_back(xn_1);
  }
  
  void increment_block_samples_counter() {
    if (block_samples == block_prev.size() - 1) {
      // block samples was filled
      // update M
      calculate_new_m();
      
    }
    block_samples = (block_samples + 1) % block_prev.size(); // 0 a 1023
    
  }
  
  void calculate_new_m() {
    // key : sample
    // value : probability
    std::map<short, double> probabilities;
    for (auto c: block_prev) {
      probabilities[c]++; //  count appearances
    }
    for (auto &entry: probabilities) {
      probabilities[entry.first] = entry.second / block_samples;
    }
    // this was tested!
    
    
    // calculate M
    double m = 0;
    for (auto &entry: probabilities) {
      m += ceil(log2(2 - entry.second) / log2(1 - entry.second));
    }
    m /= block_prev.size();
    
    // round M to the nearest power of 2
    coder.setM((int) pow(2, ceil(log2(m))));
  }


public:
  Encoder(int m, int block) {
    coder.setM(m);
    block_prev.resize(3);
    block_prev.resize(block);
    
    std::fill(block_prev.begin(), block_prev.end(), 0);
    std::fill(block_prev.begin(), block_prev.end(), 0);
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
    //std::ofstream f1 {"Encoded_residuos", std::ofstream::app};
    //std::ofstream f2 {"residuos", std::ofstream::app};
    
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
    
    if (sndFile.channels() != 2) {
      std::cerr << "ERRRRRRRRRO";
      std::exit(-1);
    }
    
    while ((nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) { // 10 2
      samples.resize(nFrames * sndFile.channels());
      for (l = 0, r = 1; l < samples.size() - 1 && r < samples.size(); r += 2, l += 2) {
        c++;
        left_sample = samples[l];
        right_sample = samples[r];
        l_r = left_sample + right_sample;
  
        mid = (left_sample + right_sample) / 2;
        side = left_sample - right_sample;
        
        // TODO: check if mid is odd
        if ( l_r % 2 == 1){
          l_r = 2 *
        }
  
        
        add_new_sample_block(mid - predict());
        
        encode_residual(mid - predict(), bits);
        
        write_to_file(bits, bitStream);
        //std::cout << "MID: " << mid << std::endl;
        //std::cout << "MID - pred : " << mid - predict() << std::endl;
        //std::cout << bits << std::endl;
        bits.clear();
        encode_residual(side, bits);
        write_to_file(bits, bitStream);
        //std::cout << "SIDE: " << side << std::endl;
        //std::cout << bits << std::endl;
        
        //std::cout << "Right: " << right_sample << std::endl;
        //std::cout << "Left: " << left_sample << std::endl;
        
        
        add_new_sample(mid);
      }
      /*
      for (auto c: samples) {
        
        residual = c - predict();
        std::cout << "RES: " << residual << std::endl;
        std::cout << "S: " << c << std::endl;
        std::cout << "Res: " << predict() << std::endl;
        add_new_sample(c);
        encode_residual(residual, bits);
        //std::cout << bits << "|" << std::endl;
        
        write_to_file(bits, bitStream);
        i++;
      }
      */
      
    }
    
    
    //std::cout << "Escrever ficheiro " << sndFile.frames() << std::endl;
    //std::cout << "Escrever ficheiro " << c << std::endl;
    sndFile.writef(samples.data(), samples.size() / sndFile.channels());
    
    bitStream.close();
  }
};


#endif //P2_ENCODER_H
