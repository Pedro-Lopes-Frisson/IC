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

public:
  // Inicialize the class to decode
  imgDecoder() {
  
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
    //int num_count_size = 0;
    
    // Run through necessary bits in the coded file to get the img size
    //while(num_count_size !=  2){
    for (int num_count_size = 0; num_count_size < 2; num_count_size++) {
      
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
      int reminder_size = ceil(log2((int) pow(2, 10)));
      reminder_int.resize(reminder_size);
      
      // Read the amount of bits
      bitStream.getNBit(reminder_int.data(), reminder_size);
      
      //Create the string of reminder
      std::string remainder;
      for (int x = 0; x < reminder_size; x++) {
        if (reminder_int[x] == 1) {
          remainder += one_num;
        } else if (reminder_int[x] == 0) {
          remainder += zero_num;
        }
      }
      
      // Concatenare q and r
      std::string q_r = quocient + remainder;
      // Make the decodation
      long long int size;
      coder.decode_int(&size, q_r);
      
      if (num_count_size == 0) {
        height = size;
        std::cout << height << std::endl;
        bit = bitStream.getBit();
      } else if (num_count_size == 1) {
        width = size;
        std::cout << width << std::endl;
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
            if(bit == EOF)
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
              if(bit == EOF)
                break;
            }
            q_flag = 0;
            // Read reminder
            // Since bitStream reads integers, we need a vector
            std::vector<int> r_R_int;
            int reminder_size = 10;
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
              //std::cout << remainder << std::endl;
            }
            
            // Concatenare q and r
            std::string q_r = quocient + remainder;
            // Make the decodation
            long long int dec;
            coder.decode_int(&dec, q_r);
            // Get the pixel
            //int pixel;
            // Ill use the prediction just to see if works
            //std::cout << img_out.at<cv::Vec3b>(i,j) << std::endl;
            // Write pixel
            //std::cout << pred << std::endl;
            
            if (k == 0) {
              error_B.at<uchar>(i, j) = dec;
              std::cout << "B: " << q_r << std::endl;
            } else if (k == 1) {
              error_G.at<uchar>(i, j) = dec;
              std::cout << "G: " << q_r << std::endl;
            } else {
              error_R.at<uchar>(i, j) = dec;
              std::cout << "R: " << q_r << std::endl;
            }
          }
          if(bit == EOF)
            break;
        }
        if(bit == EOF)
          break;
      }
      if(bit == EOF)
        break;
    }
    JPEG_LS_predictor(error_B, img_out_B);
    JPEG_LS_predictor(error_G, img_out_G);
    JPEG_LS_predictor(error_R, img_out_R);
    //std::cout << img_out_B.at<uchar>(0,0) << std::endl;
    
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        img_out.at<cv::Vec3b>(i, j)[0] = img_out_B.at<uchar>(i, j);
        img_out.at<cv::Vec3b>(i, j)[1] = img_out_G.at<uchar>(i, j);
        img_out.at<cv::Vec3b>(i, j)[2] = img_out_R.at<uchar>(i, j);
      }
    }
    // Write in to the file
    cv::imwrite(outFile, img_out);
    //cv::imshow("Decoded", img_out);
    //cv::waitKey();
    // Close the bitStream
    bitStream.close();
  }
};

#endif
