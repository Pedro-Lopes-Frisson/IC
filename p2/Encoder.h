#ifndef P2_ENCODER_H
#define P2_ENCODER_H

#include "BitStream.h"
#include "GolombCoding.h"
#include <string>
#include <sndfile.hh>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>


class Encoder {
private:
  GolombCoder coder{(int) pow(2, 16)};
  const static long FRAMES_BUFFER_SIZE = 65536;
  
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

public:
  Encoder() {
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
    short lastValue = 0;
    std::string bits;
    //std::ofstream f1 {"Encoded_residuos", std::ofstream::app};
    //std::ofstream f2 {"residuos", std::ofstream::app};
    
    // Percorrer as samples todas
    long long int i = 0;
    while ((nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) { // 10 2
      samples.resize(nFrames * sndFile.channels());
      for (auto c: samples) {
        
        residual = c - lastValue;
        lastValue = c;
        encode_residual(residual, bits);
        
        write_to_file(bits, bitStream);
        i++;
        std::cout << c << std::endl;
      }
      
    }
    
    
    bitStream.close();
  }
};


#endif //P2_ENCODER_H
