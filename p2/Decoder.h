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
#include <map>


class Decoder {
private:
  std::vector<short> block_prev_mid;
  std::vector<short> block_prev_side;
  GolombCoder coder_mid{10};
  GolombCoder coder_side{10};
  std::vector<short> lastvalues;
  std::vector<short> lastvaluesSide;
  int block_samples_mid = 0;
  int block_samples_side = 0;
  
  
  int open_wav(SndfileHandle &inFile, std::string filename, int channels, long long int srate) {
    inFile = SndfileHandle(filename.data(), SFM_WRITE, 65538, channels, srate);
    
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
  
  int predict(int mid) {
    if (mid) {
      return lastvalues[0] - 3 * lastvalues[1] + 3 * lastvalues[2];
    } else {
      return lastvaluesSide[0] - 3 * lastvaluesSide[1] + 3 * lastvaluesSide[2];
    }
    
  }
  
  //int encode_residual(short value, std::string &bits) {
  void decode_residiual(long long int *value, std::string &bits, int mid) {
    if (mid)
      coder_mid.decode_int(value, bits);
    else
      coder_side.decode_int(value, bits);
  };
  
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
  
  
  void add_new_sample(short xn_1, int mid) {
    if (mid) {
      lastvalues.erase(lastvalues.begin()); // remove the oldest predictor
      lastvalues.push_back(xn_1);
    } else {
      lastvaluesSide.erase(lastvaluesSide.begin()); // remove the oldest predictor
      lastvaluesSide.push_back(xn_1);
    }
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
      //std::cout << std::endl << "Mid, Previous M: " << coder_mid.M;
      if (sample_mean > 0)
        m = ceil(log2(sample_mean / 2));
      
      coder_mid.M = pow(2, m);
      if (m <= 0)
        coder_mid.M = 1;
      //std::cout << std::endl << "MID, Next M: " << coder_mid.M;
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
    //std::cout << std::endl << "side, Previous M: " << coder_side.M;
    if (sample_mean > 0)
      m = ceil(log2(sample_mean / 2));
    
    coder_side.M = pow(2, m);
    if (m <= 0)
      coder_side.M = 1;
    //std::cout << "m: " << m;
    //std::cout << std::endl << "Side, Next M: " << coder_side.M;
  }


public:
  Decoder(int k, int b) {
    coder_side.M = pow(2, k);
    coder_mid.M = pow(2, k);
    block_samples_mid = 0;
    block_samples_side = 0;
    block_prev_side.resize(b);
    block_prev_mid.resize(b);
    
    lastvalues.resize(3);
    lastvaluesSide.resize(3);
    std::fill(lastvaluesSide.begin(), lastvaluesSide.end(), 0);
    std::fill(lastvalues.begin(), lastvalues.end(), 0);
    std::fill(block_prev_side.begin(), block_prev_side.end(), 0);
    std::fill(block_prev_mid.begin(), block_prev_mid.end(), 0);
  }
  
  // inFile é o ficheiro codificado - outFile é o ficheiro descodificado
  void decode_Encoded_audio_file(std::string inFile, std::string outFile) {
    SndfileHandle sfhOut;
    
    // Abrir o ficheiro codificado
    BitStream bitStream = BitStream(inFile.data(), BT_READ);
    // Vetor para armazenar os residuos
    std::vector<short> samples;
    
    // Ler cada um dos bits
    
    // Criar o ficheiro para escrever o ficheiro de áudio
    /*
    long long int frames;
    long long int samplerate;
    int channels;
     */
    
    
    const char one_num = '1';
    const char zero_num = '0';
    std::string q_r;
    long long int count = 0;
    long long int mid, side, residual, residual_side;
    short left_sample, right_sample;
    //samples.resize(2);
    int eof_flag = 0;
    std::ofstream f1{"res_bits_1", std::ofstream::app};
    std::ofstream f2{"side_bits_1", std::ofstream::app};
    std::vector<int> remainder_bits;
    short nChannels = 0;
    long long int sampleRate = 0;
    int bits_sR[16];
    bitStream.getNBit(bits_sR, 16);
    for (int h = 0; h < 16; h++) {
      if (bits_sR[h])
        sampleRate += pow(2, 15 - h);
    }
    
    
    int bits_nChannels[8];
    bitStream.getNBit(bits_nChannels, 8);
    
    for (int h = 0; h < 8; h++) {
      if (bits_nChannels[h])
        nChannels += pow(2, 7 - h);
    }
    
    
    if (nChannels == 2) {
      // 2 channels decode both M parameter
      bitStream.getNBit(bits_nChannels, 8);
      coder_mid.M = 0;
      for (int h = 0; h < 8; h++) {
        if (bits_nChannels[h]) // get initial M
          coder_mid.M += pow(2, 7 - h);
      }
      coder_mid.M = pow(2, coder_mid.M);
      
      bitStream.getNBit(bits_nChannels, 8);
      coder_side.M = 0;
      for (int h = 0; h < 8; h++) {
        if (bits_nChannels[h]) // get initial M
          coder_side.M += pow(2, 7 - h);
      }
      coder_side.M = pow(2, coder_side.M);
      
    } else {
      // only 1 channel decode only one M parameter
      bitStream.getNBit(bits_nChannels, 8);
      coder_mid.M = 0;
      for (int h = 0; h < 8; h++) {
        if (bits_nChannels[h]) // get initial M
          coder_mid.M += pow(2, 7 - h);
      }
      coder_mid.M = pow(2, coder_mid.M);
    }
    
    
    int res = open_wav(sfhOut, outFile, nChannels, sampleRate);
    if (res != 0) {
      std::cerr << "File could not be open" << std::endl;
    }
    
    
    //Decode samplerate and nChannels
    if (nChannels == 2) {
      
      auto bit = bitStream.getBit();
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
        if (eof_flag) {
          break;
        }
        
        // read remainder
        int read_bits;
        if (count % 2 == 0) {
          read_bits = log2(coder_mid.M);
        } else {
          read_bits = log2(coder_side.M);
        }
        
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
        
        if (count % 2 == 0) {
          f1 << q_r << std::endl;
          decode_residiual(&residual, q_r, 1);
          mid = predict(1) + residual; // recover what was lost from encoding only the residual
          //std::cout << "MID: " << mid << std::endl;
          //std::cout << "MID Bits: " << q_r << std::endl;
        } else if (count % 2 == 1) {
          decode_residiual(&residual_side, q_r, 0);
          f2 << q_r << std::endl;
          side = predict(0) + residual_side;
          // now we have everything we need to recreate the audio channels
          //std::cout << "SIDE: " << side << std::endl;
          //std::cout << "SIDE Bits: " << q_r << std::endl;
          // if mid was odd it means it was changed
          double h_side = side / (double) 2;
          
          left_sample = ceil(mid + h_side);
          right_sample = ceil(mid - h_side);
          
          //std::cout << count << std::endl;
          //samples.push_back(left_sample);
          //samples.push_back(right_sample);
          //std::cout << "S: " << s << std::endl;
          samples.push_back(left_sample);
          samples.push_back(right_sample);
          
          //s += 2;
          //std::cout << "SIDE: " << side << std::endl;
          //std::cout << "MID: " << mid << std::endl;
          //std::cout << "Right: " << right_sample << std::endl;
          //std::cout << "Left: " << left_sample << std::endl;
          
          // add value read from file
          add_new_sample_block_mid(residual);
          add_new_sample_block_side(residual_side);
          // add recovered value predict + value read from file
          add_new_sample(mid, 1);
          add_new_sample(side, 0);
          
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
    } else {
      
      auto bit = bitStream.getBit();
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
        if (eof_flag) {
          break;
        }
        
        // read remainder
        int read_bits;
        read_bits = log2(coder_mid.M);
        
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
        decode_residiual(&residual, q_r, 1);
        f2 << q_r << std::endl;
        mid = predict(1) + residual;
        // now we have everything we need to recreate the audio channels
        std::cout << "MID: " << mid << std::endl;
        std::cout << "MID Bits: " << q_r << std::endl;
        // if mid was odd it means it was changed
        
        //std::cout << count << std::endl;
        //samples.push_back(left_sample);
        //samples.push_back(right_sample);
        //std::cout << "S: " << s << std::endl;
        samples.push_back(mid);
        
        
        // add value read from file
        add_new_sample_block_mid(residual);
        // add recovered value predict + value read from file
        add_new_sample(mid, 1);
        
        
        count++;
        
        //sample = pred + predict();
        
        //std::cout << "Sample Real, " << count++ << ": " << sample << "\n";
        //std::cout << "Encode : " << pred << "\n";
        //add_new_sample(sample);
        
        //std::cout << q_r << "|" << std::endl;
        bit = bitStream.getBit();
        q_r.clear();
      }
    }
    
    
    //std::cout << "Escrever ficheiro" << s << std::endl;
    //std::cout << "Escrever ficheiro" << count << std::endl;
    //std::cout << "Escrever ficheiro " << samples.size() << std::endl;
    // Escrever no ficheiro .wav
    bitStream.close();
    sfhOut.writef(samples.data(), samples.size() / sfhOut.channels());
    
    
  }
  
};

#endif //P2_DECODER_H
