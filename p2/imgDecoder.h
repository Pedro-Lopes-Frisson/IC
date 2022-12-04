#ifndef P2_IMGDECODER_H
#define P2_IMGDECODER_H

#include "BitStream.h"
#include "GolombCoding.h"
#include <string>
#include <sndfile.hh>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <string.h>
#include <algorithm>


class imgDecoder {
private:
  // Defining the Golomb
  GolombCoder coder{(int) pow(2, 10)};
  GolombCoder coder_R{(int) pow(2, 10)};
  GolombCoder coder_G{(int) pow(2, 10)};
  GolombCoder coder_B{(int) pow(2, 10)};
  // Defening vectors for each channel
  std::vector<int> block_prev_R;
  std::vector<int> block_prev_G;
  std::vector<int> block_prev_B;
  // Pixel channel counters
  size_t R_channel_counter = 0;
  size_t G_channel_counter = 0;
  size_t B_channel_counter = 0;
  
  int predict(int a, int b, int c) {
    
    int p;
    if (c >= std::max(a, b)) {
      p = std::min(a, b);
    } else if (c <= std::min(a, b)) {
      p = std::max(a, b);
    } else {
      p = a + b - c;
    }
    
    return p;
  }
  
  void JPEG_LS_predictor(cv::Mat erro, cv::Mat &desc) {
    cv::Size s = erro.size();
    for (int i = 0; i < s.height; i++) {
      for (int j = 0; j < s.width; j++) {
        int a, b, c;
        
        int error = erro.at<uchar>(i, j);
        
        if (j == 0 && i != 0) {
          a = 0;
          b = erro.at<uchar>(i - 1, j);
          c = 0;
        } else if (j != 0 && i == 0) {
          a = erro.at<uchar>(i, j - 1);
          b = 0;
          c = 0;
        } else if (j == 0 && i == 0) {
          a = 0;
          b = 0;
          c = 0;
        } else {
          a = erro.at<uchar>(i, j - 1);
          b = erro.at<uchar>(i - 1, j);
          c = erro.at<uchar>(i - 1, j - 1);
        }
        
        int prediction = predict(a, b, c);
        
        int pixel = pixelDec(error, prediction);
        
        desc.at<uchar>(i, j) = pixel;
        erro.at<uchar>(i, j) = pixel;
      }
    }
  }
  
  int pixelDec(int erro, int prediction) {
    return erro + prediction;
  }


  void add_new_value(int value, short channel){
    // Blue channel
    if(channel == 0){
      increment_pixel_channel_counter(channel);
      block_prev_B.erase(block_prev_B.begin());
      block_prev_B.push_back(value);
    }
    // Green Channel
    else if(channel == 1){
      increment_pixel_channel_counter(channel);
      block_prev_G.erase(block_prev_G.begin());
      block_prev_G.push_back(value);
    }
    // Red Channel
    else{
      increment_pixel_channel_counter(channel);
      block_prev_R.erase(block_prev_R.begin());
      block_prev_R.push_back(value);
    }
  }

  void increment_pixel_channel_counter(short channel){
    // Blue channel
    if(channel == 0){
      // Vector that have size of block is full, new M can be calculated
      if(B_channel_counter == block_prev_B.size() - 1){
        // Update M
        calculate_new_M(channel);
      }
      // Increment the counter
      B_channel_counter = (B_channel_counter + 1) % block_prev_B.size();
      return;
    }
    // Green Channel
    else if(channel == 1){
      // Vector that have size of block is full, new M can be calculated
      if(G_channel_counter == block_prev_G.size() - 1){
        // Update M
        calculate_new_M(channel);
      }
      // Increment the counter
      G_channel_counter = (G_channel_counter + 1) % block_prev_G.size();
      return;
    }
    // Red Channel
    else{
      // Vector that have size of block is full, new M can be calculated
      if(R_channel_counter == block_prev_R.size() - 1){
        // Update M
        calculate_new_M(channel);
      }
      // Increment the counter
      R_channel_counter = (R_channel_counter + 1) % block_prev_R.size();
      return;
    }
  }

  void calculate_new_M(short channel){
    // Blue channel
    if(channel == 0){
      double B_channel_mean = 0;
      // Run througth every value of channel
      for(auto v: block_prev_B){
        B_channel_mean += 2 * v;
      }
      B_channel_mean /= block_prev_B.size();

      // Get M
      double K = 0;
      if(B_channel_mean > 1){
        K = ceil(log2(B_channel_mean / 2));
        //std::cout << "M_B: " << pow(2, K) << std::endl;
        coder_B.M = pow(2, K);
      }
      else{
      coder_B.M = 1;
      }
      return;

    }
    // Green Channel
    else if(channel == 1){
      double G_channel_mean = 0;
      // Run througth every value of channel
      for(auto v: block_prev_G){
        G_channel_mean += 2 * v;
      }
      G_channel_mean /= block_prev_G.size();

      // Get M
      double K = 0;
      if(G_channel_mean > 1){
        K = ceil(log2(G_channel_mean / 2));
        //std::cout << "M_G: " << pow(2, K) << std::endl;
        coder_G.M = pow(2, K);
      }
      else{
      coder_G.M = 1;
      }
      return;

    }
    // Red Channel
    else{
      double R_channel_mean = 0;
      // Run througth every value of channel
      for(auto v: block_prev_R){
        R_channel_mean += 2 * v;
      }
      R_channel_mean /= block_prev_R.size();

      // Get M
      double K = 0;
      if(R_channel_mean > 1){
        K = ceil(log2(R_channel_mean / 2));
        //std::cout << "M_R: " << pow(2, K) << std::endl;
        coder_R.M = pow(2, K);
      }
      else{
      coder_R.M = 1;
      }
      return;
    }
  }


public:
  // Inicialize the class to decode
  imgDecoder() {
    block_prev_R.resize(50);
    block_prev_G.resize(50);
    block_prev_B.resize(50);

    R_channel_counter = 0;
    G_channel_counter = 0;
    B_channel_counter = 0;

    std::fill(block_prev_R.begin(), block_prev_R.end(), 0);
    std::fill(block_prev_G.begin(), block_prev_G.end(), 0);
    std::fill(block_prev_B.begin(), block_prev_B.end(), 0);
  
  }
  
  // Function to decode the image
  void decode_coded_image(std::string inFile, std::string outFile) {
    
    // Open the BitStream
    BitStream bitStream = BitStream(inFile.data(), BT_READ);
    
    // Read the first bit of the coded file
    auto bit = bitStream.getBit();
    
    // Boolean variable to read quocient q
    bool q_flag = 0;
    // Chars to get unary code
    const char one_num = '1';
    const char zero_num = '0';
    // Variable to controll the channel we are in
    int channel_counter = 0;
    
    // To decode the image size
    long long int height;
    long long int width;
    
    // Run through necessary bits in the coded file to get the img size
    //while(num_count_size !=  2){
    for (int num_count_size = 0; num_count_size < 5; num_count_size++) {
      
      std::string quocient;
      // Read Quocient
      while (q_flag == 0) {
        
        if (bit == 0) {
          q_flag = 1;
          quocient += zero_num;
          break;
        } else if (bit == 1) {
          quocient += one_num;
        }
        bit = bitStream.getBit();
      }
      //Read remeinder
      std::vector<int> reminder_int;
      // Tenho de calcular aqui o M
      int reminder_size = ceil(log2((int) pow(2, 10)));
      reminder_int.resize(reminder_size);
      
      // Read the amount of bits
      //bitStream.getNBit(reminder_int.data(), reminder_size);
      
      //Create the string of reminder
      std::string remainder;
      for (int x = 0; x < reminder_size; x++) {
        bit = bitStream.getBit();
        if (bit == 1) {
          remainder += one_num;
        } else if (bit == 0) {
          remainder += zero_num;
        }
        if (bit == EOF) {
          std::cerr << "EOF";
          break;
        }
      }
      if (bit == EOF) {
        break;
      }
      
      // Concatenare q and r
      std::string q_r = quocient + remainder;
      // Make the decodation
      long long int size;
      coder.decode_int(&size, q_r);
      
      if (num_count_size == 0) {
        height = size;
        //std::cout << height << std::endl;
        bit = bitStream.getBit();
      } else if (num_count_size == 1) {
        width = size;
        //std::cout << width << std::endl;
        bit = bitStream.getBit();
      } else if (num_count_size == 2){
        coder_R.M = size;
        //std::cout << coder_R.M << std::endl;
        bit = bitStream.getBit();
      } else if (num_count_size == 3){
        coder_G.M = size;
        //std::cout << coder_G.M << std::endl;
        bit = bitStream.getBit();
      } else if (num_count_size == 4){
        coder_B.M = size;
        //std::cout << coder_B.M << std::endl;
      }
      
      // Clear varaibles
      quocient.clear();
      remainder.clear();
      q_r.clear();
      q_flag = 0;
      //Read new bit from next iteration
    }
    
    // We need the image size
    // Vou fazer sem isto para ver se funciona
    // Define the matrix to the outfile
    cv::Mat error_B = cv::Mat::zeros(cv::Size(width, height), CV_8UC1);
    cv::Mat error_G = cv::Mat::zeros(cv::Size(width, height), CV_8UC1);
    cv::Mat error_R = cv::Mat::zeros(cv::Size(width, height), CV_8UC1);
    
    cv::Mat img_out = cv::Mat::zeros(cv::Size(width, height), CV_8UC3);
    cv::Mat img_out_B = cv::Mat::zeros(cv::Size(width, height), CV_8UC1);
    cv::Mat img_out_G = cv::Mat::zeros(cv::Size(width, height), CV_8UC1);
    cv::Mat img_out_R = cv::Mat::zeros(cv::Size(width, height), CV_8UC1);
    
    // Run through every bit in the coded file
    while (bit != EOF) {
      
      // For loop to get the pixels in the right position
      for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
          for (int k = 0; k < 3; k++) {
            std::string quocient;
            bit = bitStream.getBit();
            if (bit == EOF)
              break;
            
            // Read quocient
            while (q_flag == 0) {
              if (bit == 0) {
                q_flag = 1;
                quocient += zero_num;
                break;
              } else if (bit == 1) {
                quocient += one_num;
              }
              bit = bitStream.getBit();
              if (bit == EOF)
                break;
            }
            q_flag = 0;
            // Read reminder
            // Since bitStream reads integers, we need a vector
            std::vector<int> r_R_int;
            //int reminder_size = 10;
            int reminder_size = 11 - quocient.size();
            r_R_int.resize(reminder_size);
            
            // Read the amount of bits
            bitStream.getNBit(r_R_int.data(), reminder_size);
            
            //Create the string of reminder
            std::string remainder;
            for (int x = 0; x < reminder_size; x++) {
              if (r_R_int[x] == 1) {
                remainder += one_num;
              } else if (r_R_int[x] == 0) {
                remainder += zero_num;
              }
              if (r_R_int[x] == EOF) {
                break;
              }
              //std::cout << remainder << std::endl;
            }
            if (r_R_int[reminder_size - 1] == EOF) {
              break;
            }
            
            // Concatenare q and r
            std::string q_r = quocient + remainder;
            // Make the decodation
            long long int dec;
            //coder.decode_int(&dec, q_r);
            
            if (k == 0) {
              coder_B.decode_int(&dec, q_r);
              error_B.at<uchar>(i, j) = dec;
              std::cout << "B: " << dec << ", B_bits: " << q_r << std::endl;
              add_new_value((int) img_out_B.at<uchar>(i, j), 0);
            } else if (k == 1) {
              coder_G.decode_int(&dec, q_r);
              error_G.at<uchar>(i, j) = dec;
              std::cout << "G: " << dec << ", G_bits: " << q_r << std::endl;
              add_new_value((int) img_out_G.at<uchar>(i, j), 1);
            } else {
              coder_R.decode_int(&dec, q_r);
              error_R.at<uchar>(i, j) = dec;
              std::cout << "R: " << dec << ", R_bits: " << q_r << std::endl;
              add_new_value((int) img_out_R.at<uchar>(i, j), 2);
            }
          }
          if (bit == EOF)
            break;
        }
        if (bit == EOF)
          break;
      }
      if (bit == EOF)
        break;
    }
    std::cout << "chega aqui ?" << std::endl;
    JPEG_LS_predictor(error_B, img_out_B);
    JPEG_LS_predictor(error_G, img_out_G);
    JPEG_LS_predictor(error_R, img_out_R);
    //std::cout << img_out_B.at<uchar>(0,0) << std::endl;
    
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        img_out.at<cv::Vec3b>(i, j)[0] = img_out_B.at<uchar>(i, j);
        // Add the encode value of the channel to the vector
        //add_new_value((int) img_out_B.at<uchar>(i, j), 0);
        std::cout << "B: " << img_out_B.at<uchar>(i, j) << std::endl;

        img_out.at<cv::Vec3b>(i, j)[1] = img_out_G.at<uchar>(i, j);
        // Add the encode value of the channel to the vector
        //add_new_value((int) img_out_G.at<uchar>(i, j), 1);
        std::cout << "G: " << img_out_G.at<uchar>(i, j) << std::endl;

        img_out.at<cv::Vec3b>(i, j)[2] = img_out_R.at<uchar>(i, j);
        // Add the encode value of the channel to the vector
        //add_new_value((int) img_out_R.at<uchar>(i, j), 2);
        std::cout << "R: " << img_out_R.at<uchar>(i, j) << std::endl;
      }
    }
    // Write in to the file
    cv::imwrite(outFile, img_out);
    // Close the bitStream
    bitStream.close();
  }
};

#endif
