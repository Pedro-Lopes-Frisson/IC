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
  
  void decode_residual(int *value, std::string &bits) {
    int sucss;
    sucss = coder.decode_int(value, bits);
    
    if (sucss != 0) {
      std::cerr << "Sample decoding could not be performed!" << std::endl;
    }
  };
  
  int predict() {
    return lastvalues[0] - 3 * lastvalues[1] + 3 * lastvalues[2];
  }
  
  void add_new_sample(short xn_1) {
    lastvalues.erase(lastvalues.begin()); // remove the oldest predictor
    lastvalues.push_back(xn_1);
  }


public:
  Decoder() {
    lastvalues.resize(3);
    std::fill(lastvalues.begin(), lastvalues.end(), 0);
    std::cout << "Pred: " << predict() << std::endl;
  }
  
  // inFile é o ficheiro codificado - outFile é o ficheiro descodificado
  void decode_Encoded_audio_file(std::string inFile, std::string outFile) {
    
    // Abrir o ficheiro codificado
    BitStream bitStream = BitStream(inFile.data(), BT_READ);
    // Vetor para armazenar os residuos
    std::vector<short> samples;
    // Ler cada um dos bits
    auto bit = bitStream.getBit();
    // bits do q
    std::string q_bits;
    // bits do r
    std::string r_bits;
    //  Flag do q
    int flag_q = 1;
    // String do qr final
    std::string qr_bits;
    // Valor do residual
    int sample;
    int atual_residual;
    // Valor do last residual
    int last_residual = 0;
    
    // Criar o ficheiro para escrever o ficheiro de áudio
    SndfileHandle sfhOut{outFile, SFM_WRITE, 65538, 2, 44100};
    
    
    long long int i = 0;
    /*
    while (bit != EOF) {
      
      qr_bits = qr_bits + std::to_string(bit);
      //std::cout << bit;
      
      if (qr_bits.size() == 17) {
        
        int sucss = coder.decode_int(&atual_residual, qr_bits);
        
        sample = atual_residual + last_residual;
        xm-1 a sample real anterior
        
        y = xm - xm-1
        - xm = - y - xm-1
        xm = y + xm-1
        //std::cout << atual_residual << std::endl;
        std::cout << "Sample" << i << ": " << sample << std::endl;
        std::cout << "Last Sample: " << last_residual << std::endl;
        std::cout << "Bits: " << qr_bits << std::endl;
        last_residual = sample;
        samples.push_back(sample);
        qr_bits.clear();
        i++;
      }
      
      // Ler o próximo bit
      bit = bitStream.getBit();
    }
     */
    long long int frames;
    long long int samplerate;
    int channels;
    
    
    const char one_num = '1';
    const char zero_num = '0';
    bool q_flag = 0;
    std::string q_r;
    int last_sample = 0;
    long long int count = 0;
    int cc = 0;
    while (bit != EOF) {
      // read q
      while (q_flag == 0) { // break only if unary code is finished
        if (bit == 0) {
          // unary code has finished
          q_flag = 1;
          q_r += zero_num;
          break;
        } else if (bit == 1) {
          q_r += one_num;
        }
        bit = bitStream.getBit();
      }
      
      // read remainder
      // read 2 bits if both are not 1's read 14 more
      int bits_2[2];
      int read_bits = log2(10000);
      bitStream.getNBit(bits_2, 2);
      std::string r_str;
      
      if (bits_2[0] == bits_2[1] == 1) {
        // read 2 bits if they are both 1's read 15 more
        read_bits = ceil(log2(10000));
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
      
      int pred;
      coder.decode_int(&pred, q_r);
      sample = pred + predict();
      
      //std::cout << "Sample Real, " << count++ << ": " << sample << "\n";
      //std::cout << "Encode : " << pred << "\n";
      add_new_sample(sample);
      samples.push_back(sample);
      
      std::cout << q_r << "|" << std::endl;
      bit = bitStream.getBit();
      q_r.clear();
      q_flag = 0;
    }
    //std::cout << "Escrever ficheiro" << std::endl;
    //std::cout << "Escrever ficheiro " << samples.size() << std::endl;
    // Escrever no ficheiro .wav
    sfhOut.writef(samples.data(), 1294041);
    bitStream.close();
    
  }
};

#endif //P2_DECODER_H
