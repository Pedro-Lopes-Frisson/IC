#ifndef P2_DECODER_H
#define P2_DECODER_H

#include "BitStream.h"
#include "GolombCoding.h"
#include <string>
#include <sndfile.hh>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>


class Decoder {
private:
  GolombCoder coder{10000};
  std::vector<short> lastvalues;
  std::vector<short> block;
  int blocksize;
  
  
  int open_wav(SndfileHandle &inFile, std::string filename) {
    inFile = SndfileHandle(filename.data(), SFM_WRITE, 65538, 2, 44100);
    
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
  
  int predict() {
    return lastvalues[0] - 3 * lastvalues[1] + 3 * lastvalues[2];
  }
  
  void add_new_sample(short xn_1) {
    lastvalues.erase(lastvalues.begin()); // remove the oldest predictor
    lastvalues.push_back(xn_1);
  }


public:
  Decoder(int m, int b) {
    coder.setM(m);
    blocksize = b;
    block.resize(blocksize);
    
    lastvalues.resize(3);
    std::fill(lastvalues.begin(), lastvalues.end(), 0);
  }
  
  // inFile é o ficheiro codificado - outFile é o ficheiro descodificado
  void decode_Encoded_audio_file(std::string inFile, std::string outFile) {
    SndfileHandle sfhOut;
    
    // Abrir o ficheiro codificado
    BitStream bitStream = BitStream(inFile.data(), BT_READ);
    // Vetor para armazenar os residuos
    std::vector<short> samples;
    // Ler cada um dos bits
    auto bit = bitStream.getBit();
    
    // Criar o ficheiro para escrever o ficheiro de áudio
    int res = open_wav(sfhOut, outFile);
    if (res != 0) {
      std::cerr << "File could not be open" << std::endl;
    }
    /*
    long long int frames;
    long long int samplerate;
    int channels;
     */
    
    
    const char one_num = '1';
    const char zero_num = '0';
    std::string q_r;
    long long int count = 2;
    long long int mid, side;
    short left_sample, right_sample;
    int s = 0;
    //samples.resize(2);
    int eof_flag = 0;
    
    
    while (bit != EOF) {
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
      if(eof_flag){
        break;
      }
      
      // read remainder
      // read 2 bits if both are not 1's read 14 more
      int bits_2[2];
      int read_bits = log2(1024) - 2;
      bitStream.getNBit(bits_2, 2);
      if ((bits_2[0] == bits_2[1]) == 1) {
        // read 2 bits if they are both 1's read 15 more
        read_bits = ceil(log2(1024)) - 2;
      }
      std::vector<int> remainder_bits;
      remainder_bits.resize(read_bits);
      
      bitStream.getNBit(remainder_bits.data(), read_bits);
      // create binary string
      for (int j = 0; j < read_bits + 2; j++) {
        if (j < 2) {
          if (bits_2[j] == 1) {
            q_r += one_num;
          } else {
            q_r += zero_num;
          }
        } else {
          if (remainder_bits[j - 2] == 1) {
            q_r += one_num;
          } else {
            q_r += zero_num;
          }
        }
      }
      /*
      if(cc == 0){
        coder.decode_int(&frames, q_r);
        cc++;
        continue;
      }
      if(cc == 1){
        coder.decode_int(&samplerate, q_r);
        cc++;
        continue;
      }
      if(cc == 2){
        coder.decode_int(&channels, q_r);
        cc++;
        continue;
      }
       */
      
      if (count % 2 == 0) {
        coder.decode_int(&mid, q_r);
        mid = predict() + mid; // recover what was lost from encoding only the residual
        //std::cout << "MID: " << mid << std::endl;
        //std::cout << "MID Bits: " << q_r << std::endl;
      } else if (count % 2 == 1) {
        coder.decode_int(&side, q_r); // we assume this sample is already really low
        // now we have everything we need to recreate the audio channels
        //std::cout << "SIDE: " << side << std::endl;
        //std::cout << "SIDE Bits: " << q_r << std::endl;
        left_sample = mid + side;
        right_sample = mid - side;
        
        //std::cout << count << std::endl;
        //samples.push_back(left_sample);
        //samples.push_back(right_sample);
        std::cout << "S: " << s << std::endl;
        samples.push_back(left_sample);
        samples.push_back(right_sample);
        
        //s += 2;
        std::cout << "Right: " << right_sample << std::endl;
        std::cout << "Left: " << left_sample << std::endl;
        add_new_sample(mid);
        
      }
      
      count++;
      
      //sample = pred + predict();
      
      //std::cout << "Sample Real, " << count++ << ": " << sample << "\n";
      //std::cout << "Encode : " << pred << "\n";
      //add_new_sample(sample);
      
      //std::cout << q_r << "|" << std::endl;
      bit = bitStream.getBit();
      q_r.clear();
    }
    
    //std::cout << "Escrever ficheiro" << s << std::endl;
    //std::cout << "Escrever ficheiro" << count << std::endl;
    //std::cout << "Escrever ficheiro " << samples.size() << std::endl;
    // Escrever no ficheiro .wav
    bitStream.close();
    sfhOut.writef(samples.data(), samples.size() / 2);
   
    
  }
  
};

#endif //P2_DECODER_H
