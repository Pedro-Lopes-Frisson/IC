#ifndef P2_IMGENCODER_H
#define P2_IMGENCODER_H

#include "BitStream.h"
#include "GolombCoding.h"
#include <string>
#include <vector>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <string.h>
#include <fstream>
#include <iostream>


class imgEncoder {
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
  
  // Functions
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
  
  int prediction_residual(int pixel, int prediction) {
    return pixel - prediction;
  }
  
  void JPEG_LS_predictor(cv::Mat img_in, cv::Mat &erro) {
    cv::Size s = img_in.size();
    for (int i = 0; i < s.height; i++) {
      for (int j = 0; j < s.width; j++) {
        int a, b, c;
        
        int pixel = img_in.at<uchar>(i, j);
        
        if (j == 0 && i != 0) {
          a = 0;
          b = img_in.at<uchar>(i - 1, j);
          c = 0;
        } else if (j != 0 && i == 0) {
          a = img_in.at<uchar>(i, j - 1);
          b = 0;
          c = 0;
        } else if (j == 0 && i == 0) {
          a = 0;
          b = 0;
          c = 0;
        } else {
          a = img_in.at<uchar>(i, j - 1);
          b = img_in.at<uchar>(i - 1, j);
          c = img_in.at<uchar>(i - 1, j - 1);
        }
        
        int prediction = predict(a, b, c);
        
        int error = prediction_residual(pixel, prediction);
        
        erro.at<uchar>(i, j) = error;
      }
    }
  }
  
  void write_to_file(std::string bits, BitStream &bitStream) {
    for (size_t i = 0; i < bits.size(); i++) {
      bitStream.writeBit(bits[i] == '1');
    }
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
  
  // Inicialize the class to encode
  imgEncoder() {
  	block_prev_R.resize(1024);
  	block_prev_G.resize(1024);
  	block_prev_B.resize(1024);

  	this->R_channel_counter = 0;
  	this->G_channel_counter = 0;
  	this->B_channel_counter = 0;

  	std::fill(block_prev_R.begin(), block_prev_R.end(), 0);
  	std::fill(block_prev_G.begin(), block_prev_G.end(), 0);
  	std::fill(block_prev_B.begin(), block_prev_B.end(), 0);
  }
  
  // Function to encode the image
  void encode_image(std::string inFile, std::string outFile) {
    
    // Convert the string path of image into an image
    cv::Mat img_in = cv::imread(inFile);
    
    // Verify the image
    if (img_in.empty()) {
      std::cout << "\033[1;31mError: Image provided does not exist!\033[0m" << std::endl;
    }
    
    // Open the BitSream to write to file
    BitStream bitStream = BitStream(outFile.data(), BT_WRITE);
    // Inicialize / defining all variables needed
    // number os pixels of the image
    cv::Size s = img_in.size();

    // To get the pixel values and the decoded value
    std::ofstream RP{"R_pixel_enc_1024", std::ofstream::app};
    std::ofstream RR{"R_res_enc_1024", std::ofstream::app};
    std::ofstream GP{"G_pixel_enc_1024", std::ofstream::app};
    std::ofstream GR{"G_res_enc_1024", std::ofstream::app};
    std::ofstream BP{"B_pixel_enc_1024", std::ofstream::app};
    std::ofstream BR{"B_res_enc_1024", std::ofstream::app};
    
    //cv::Mat erro_R = cv::Mat::zeros(s.width, s.height, CV_8UC1);
    //cv::Mat erro_G = cv::Mat::zeros(s.width, s.height, CV_8UC1);
    //cv::Mat erro_B = cv::Mat::zeros(s.width, s.height, CV_8UC1);
    //cv::Mat img_in_R = cv::Mat::zeros(s.width, s.height, CV_8UC1);
    //cv::Mat img_in_G = cv::Mat::zeros(s.width, s.height, CV_8UC1);
    //cv::Mat img_in_B = cv::Mat::zeros(s.width, s.height, CV_8UC1);
    cv::Mat erro_R = cv::Mat::zeros(s.height, s.width, CV_8UC1);
    cv::Mat erro_G = cv::Mat::zeros(s.height, s.width, CV_8UC1);
    cv::Mat erro_B = cv::Mat::zeros(s.height, s.width, CV_8UC1);
    cv::Mat img_in_R = cv::Mat::zeros(s.height, s.width, CV_8UC1);
    cv::Mat img_in_G = cv::Mat::zeros(s.height, s.width, CV_8UC1);
    cv::Mat img_in_B = cv::Mat::zeros(s.height, s.width, CV_8UC1);
    
    for (int i = 0; i < s.height; i++) {
      for (int j = 0; j < s.width; j++) {
        img_in_R.at<uchar>(i, j) = img_in.at<cv::Vec3b>(i, j)[2];
        RP << (int) img_in.at<cv::Vec3b>(i, j)[2] << std::endl;
        img_in_G.at<uchar>(i, j) = img_in.at<cv::Vec3b>(i, j)[1];
        GP << (int) img_in.at<cv::Vec3b>(i, j)[1] << std::endl;
        img_in_B.at<uchar>(i, j) = img_in.at<cv::Vec3b>(i, j)[0];
        BP << (int) img_in.at<cv::Vec3b>(i, j)[0] << std::endl;
      }
    }
    
    // Encode the image size
    std::string bits_Height;
    std::string bits_Width;
    coder.encode_int((int) s.height, bits_Height); // Height
    write_to_file(bits_Height, bitStream);
    coder.encode_int((int) s.width, bits_Width); // Width
    write_to_file(bits_Width, bitStream);
    
    JPEG_LS_predictor(img_in_R, erro_R);
    JPEG_LS_predictor(img_in_G, erro_G);
    JPEG_LS_predictor(img_in_B, erro_B);

    // Encode inicial M's (each channel)
    std::string inicial_M_R;
    coder.encode_int(coder_R.M, inicial_M_R);
    write_to_file(inicial_M_R, bitStream);
    std::cout << "M_R: " << coder_R.M << ", Bits: " << inicial_M_R << std::endl;

    std::string inicial_M_G;
    coder.encode_int(coder_G.M, inicial_M_G);
    write_to_file(inicial_M_G, bitStream);
    std::cout << "M_G: " << coder_G.M << ", Bits: " << inicial_M_G << std::endl;

    std::string inicial_M_B;
    coder.encode_int(coder_B.M, inicial_M_B);
    write_to_file(inicial_M_B, bitStream);
    std::cout << "M_B: " << coder_B.M << ", Bits: " << inicial_M_B << std::endl;
    
    // Run through every pixel of the image
    for (int i = 0; i < s.height; i++) {
      for (int j = 0; j < s.width; j++) {
        std::string bits_R;
        std::string bits_G;
        std::string bits_B;
        
        //std::cout << "Valor: " << (int) erro_B.at<uchar>(i, j) << std::endl;
        coder_B.encode_int((int) erro_B.at<uchar>(i, j), bits_B);
        write_to_file(bits_B, bitStream);
        //std::cout << "B: " << bits_B << std::endl;
        // Add the encode value of the channel to the vector
        add_new_value((int) erro_B.at<uchar>(i, j), 0);
        BR << (int) erro_B.at<uchar>(i, j) << std::endl;

        //std::cout << "Valor: " << (int) erro_G.at<uchar>(i, j) << std::endl;
        coder_G.encode_int((int) erro_G.at<uchar>(i, j), bits_G);
        write_to_file(bits_G, bitStream);
        //std::cout << "G: " << bits_G << std::endl;
        // Add the encode value of the channel to the vector
        add_new_value((int) erro_G.at<uchar>(i, j), 1);
        GR << (int) erro_G.at<uchar>(i, j) << std::endl;

        //std::cout << "Valor: " << (int) erro_R.at<uchar>(i, j) << std::endl;
        coder_R.encode_int((int) erro_R.at<uchar>(i, j), bits_R);
        write_to_file(bits_R, bitStream);
        //std::cout << "R: " << bits_R << std::endl;
        // Add the encode value of the channel to the vector
        add_new_value((int) erro_R.at<uchar>(i, j), 2);
        RR << (int) erro_R.at<uchar>(i, j) << std::endl;

      }
    }
    bitStream.close();
    RP.close();
    RR.close();
    GP.close();
    GR.close();
    BP.close();
    BR.close();
  }
};

#endif