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
  
  void getBits(long long int n, std::vector<char> &bits) {
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
  int M;
  GolombCoder(int M) {
    this->M = M;
  }
  
  
  int decode_int(long long int *decoded_num, std::string str) {
    long long int num;
    size_t i = 0;
    int found = 0; // if 0 '0' was nhot found
    int q = 0; // quotient
    for (i = 0; i < str.size(); i++) {
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
    
    
    double k1 = log2(M);
    int k = ceil(k1);
    int c = pow(2, k) - M;
    
    
    std::string rem = str.substr(i + 1, k - 1);
    
    
    long rem_bin = 0;
    for (size_t l = 0; l < rem.size(); l++) {
      if (rem[l] == '1') {
        rem_bin += pow(2, (rem.size() - l - 1));
      }
    }
    
    
    if (rem_bin < c) {
      num = M * q + rem_bin;
    } else {
      rem = str.substr(i + 1, k);
      rem_bin = 0;
      for (size_t l = 0; l < rem.size(); l++) {
        if (rem[l] == '1') {
          rem_bin += pow(2, (rem.size() - l - 1));
        }
      }
      num = M * q;
      num += rem_bin;
      num -= c;
    }
    if (num % 2 == 0) {
      //number was positive
      num = num / 2;
    } else {
      //num was negative
      num = -(num - 1) / 2;
    }
    *decoded_num = num;
    
    return 0;
  }
  
  void encode_int(long long int num, std::string &str) {
    if (num >= 0) {
      num = 2 * num;
    } else {
      num = (abs(num) * 2) + 1;
    }
    // 1 0
    // q
    // remainder
    
    int quotient = num / M;
    int remainder = num % M;
    std::vector<char> quotient_enc;
    //allocate q + 1 bits
    quotient_enc.resize(quotient + 1);
    //Write string with q 1's
    for (size_t n = 0; n < quotient_enc.size() - 1; n++)
      quotient_enc[n] = '1';
    // write one 0
    quotient_enc[quotient_enc.size() - 1] = '0';
    // end of quotient code
    
    // start remainder code
    std::vector<char> bits_remainder;
    double k1 = log2(M);
    int k = ceil(k1);
    
    int c = pow(2, k) - M;
    
    // c = 0
    if (remainder >= 0 and remainder < c) {
      bits_remainder.resize(k - 1);
      getBits(remainder, bits_remainder);
    } else {
      bits_remainder.resize(k);
      getBits(remainder + c, bits_remainder);
    }
    
    str.resize(quotient_enc.size() + bits_remainder.size());
    int i = 0;
    
    for (auto c: quotient_enc) {
      str[i] = c;
      i++;
    }
    
    for (auto c: bits_remainder) {
      str[i] = c;
      i++;
    }
    return; // str_enc
  }
  
};

#endif //P2_GOLOMBCODING_H