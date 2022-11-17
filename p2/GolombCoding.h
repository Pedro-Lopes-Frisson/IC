//
// Created by whoknows on 10/31/22.
//

#ifndef P2_GOLOMBCODING_H
#define P2_GOLOMBCODING_H

#include <string>
#include <cmath>
#include <vector>
#include <bitset>
#include <iostream>
#include <stdlib.h>

class GolombCoder {
private:
  //int M;
  
  void getBits(int n, std::vector<char> &bits) {
    // counter for binary array
    int i = bits.size() - 1;
    while (n > 0 && i >= 0) {
      
      // storing remainder in binary array
      if (n % 2 == 0) {
        bits[i] = '0';
      } else {
        bits[i] = '1';
      }
      
      n = n / 2;
      i--;
    }
    
    for (; i >= 0; i--)
      bits[i] = '0';
    
    
  }

public:
  //GolombCoder(int M) {
  //  this->M = M;
  //}
  
  GolombCoder() {
  }
  

  //int decode_int(int *decoded_num, std::string str, int M) {
  int decode_int(int *decoded_num, std::string str) {
    
    size_t i = 0;
    int found = 0; // if 0 '0' was nhot found
    int q = 0; // quotient
    std::string str_k;

    for (i; i < str.size(); i++){
      if (str[i] == '0') {
        break;
      }
      else{
        str_k = str_k + str[i];
      }
    }

    //for (i = 0; i < str.size(); i++) {
    for (i; i < str.size(); i++) {
      if (str[i] == '0') {
        // break counting loop
        found = 1;
        break;
      } else // count 1's before 0
        q++;
    }
    
    if (found == 0) {
      std::cerr << "No Unary code was found" << std::endl;
      return -1;
    }
    
    int k = stoi(str_k);
    int M = pow(2, k);
    int c = pow(2, k) - M;
    
    
    std::string rem = str.substr(i + 1, k - 1);
    
    
    long rem_bin = 0;
    for (int l = 0; l < rem.size(); l++) {
      if (rem[l] == '1') {
        rem_bin += pow(2, (rem.size() - l - 1));
      }
    }
    
    
    if (rem_bin < c) {
      *decoded_num = M * q + rem_bin;
    } else {
      rem = str.substr(i + 1, k);
      rem_bin = 0;
      for (int l = 0; l < rem.size(); l++) {
        if (rem[l] == '1') {
          rem_bin += pow(2, (rem.size() - l - 1));
        }
      }
      *decoded_num = M * q + rem_bin - c;
    }
    if (*decoded_num % 2 == 0) {
      //number was positive
      *decoded_num = *decoded_num / 2;
    } else {
      //num was negative
      *decoded_num = -(*decoded_num - 1) / 2;
    }
    
    
    return 0;
  }



  void encode_int(int num, std::string &str) {
    if (num > 0) {
      num = 2 * num;
    }
    else {
      num = (abs(num) * 2) + 1;
    }
    
    // Calculamos o M dinamicamente
    double k1= log2(num);
    int k = floor(k1) + 1;
    int M = pow(2, k);

    int quotient = num / M;
    int remainder = num % M;
    std::vector<char> quotient_enc;
    //allocate q + 1 bits
    quotient_enc.resize(quotient + 1);
    //Write string with q 1's
    for (int n = 0; n < quotient_enc.size() - 1; n++)
      quotient_enc[n] = '1';
    // write one 0
    quotient_enc[quotient_enc.size() - 1] = '0';
    // end of quotient code
    
    // start remainder code
    std::vector<char> bits_remainder;
    int c = pow(2, k) - M;
    
    if (remainder >= 0 and remainder < c) {
      bits_remainder.resize(k - 1);
      getBits(remainder, bits_remainder);
    } else {
      bits_remainder.resize(k);
      getBits(remainder + c, bits_remainder);
    }
    
    // Concatenar o K
    std::string str_k = std::to_string(k);


    //str.resize(quotient_enc.size() + bits_remainder.size());
    str.resize(str_k.size() + quotient_enc.size() + bits_remainder.size());
    int i = 0;
    
    for (auto c: str_k) {
      str[i] = c;
      i++;
    }

    for (auto c: quotient_enc) {
      str[i] = c;
      i++;
    }
    
    for (auto c: bits_remainder) {
      str[i] = c;
      i++;
    }

    //return M; 
    return;// str_enc
  }
  
};

#endif //P2_GOLOMBCODING_H
