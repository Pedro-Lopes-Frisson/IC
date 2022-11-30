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


class imgDecoder{
private:
	// Defining the Golomb
	GolombCoder coder{(int) pow(2, 10)};

public:
	// Inicialize the class to decode
	imgDecoder() {

	}

	// Function to decode the image
	void decode_coded_image(std::string inFile, std::string outFile){

		// Open the BitStream
		BitStream bitStream = BitStream(inFile.data(), BT_READ);

		// Read the first bit of the coded file
		//auto bit = bitStream.getBit();

		// Boolean variable to read quocient q
		bool q_flag = 0;
		// Chars to get unary code
		const char one_num = '1';
          	const char zero_num = '0';
          	// Variable to controll the channel we are in
          	int channel_counter = 0;

    	      // We need the image size
      	// Vou fazer sem isto para ver se funciona
      	// Define the matrix to the outfile
      	cv::Mat img_out = cv::Mat::zeros(cv::Size(512,512), 16);

		// Run through every bit in the coded file
		//while(bit != EOF){

      		// For loop to get the pixels in the right position
      		for(int i = 0; i < 512; i++){
      			for(int j = 0; j < 512; j++){

      				// Case of red channel
      				//if(channel_counter == 0){
      					// Quociente String
      					//std::string q_R;

      					// Read quocient
      					//while(q_flag == 0){

      					//	if(bit == 0){
	      				//		q_flag = 1;
	      				//		q_R += zero_num;
	      				//		break;
      					//	}
      					//	else if(bit == 1){
      					//		q_R += one_num;
      					//	}

      					//	bit = bitStream.getBit();
      					//}

      					// Read reminder
      					// Since bitStream reads integers, we need a vector
      					//std::vector<int> r_R_int;
      					//int r_R_nBits = 11 - r_R_int.size();
      					//r_R_int.resize(r_R_nBits);

      					// Read the amount of bits
      					//bitStream.getNBit(r_R_int.data(), r_R_nBits);

      					//Create the string of reminder
      					//std::string r_R;
      					//for (int j = 0; j < r_R_nBits; j++){
      					//	if(r_R[j] == 1){
      					//		r_R += one_num;
      					//	}
      					//	else if(r_R[j] == 0){
      					//		r_R += zero_num;
      					//	}
      					//}

      					// Concatenare q and r
      					//std::string q_r_R = q_R + r_R;

      					// Make the decodation
      					//int pred;
      					//coder.decode_int(&pred, q_r_R);
      					// Get the pixel
      					//int pixel;
      					// Ill use the prediction just to see if works

      					// Write pixel
      					//img_out.at<cv::Vec3b>(i,j)[0] = pixel;

      					// Increment the channel count
      					//channel_counter++;

      					// Read a new bit
						//bit = bitStream.getBit();

      				//}// End of the channel RED

      				//else if(channel_counter == 1){
      					// Quociente String
      				//	std::string q_G;

      					// Read quocient
      				//	while(q_flag == 0){

      				//		if(bit == 0){
	      			//			q_flag = 1;
	      			//			q_G += zero_num;
	      			//			break;
      				//		}
      				//		else if(bit == 1){
      				//			q_G += one_num;
      				//		}

      				//		bit = bitStream.getBit();
      				//	}

      					// Read reminder
      					// Since bitStream reads integers, we need a vector
      				//	std::vector<int> r_G_int;
      				//	int r_G_nBits = 11 - r_G_int.size();
      				//	r_G_int.resize(r_G_nBits);

      					// Read the amount of bits
      				//	bitStream.getNBit(r_G_int.data(), r_G_nBits);

      					//Create the string of reminder
      				//	std::string r_G;
      				//	for (int j = 0; j < r_G_nBits; j++){
      				//		if(r_G[j] == 1){
      				//			r_G += one_num;
      				//		}
      				//		else if(r_G[j] == 0){
      				//			r_G += zero_num;
      				//		}
      				//	}

      					// Concatenare q and r
      				//	std::string q_r_G = q_G + r_G;

      					// Make the decodation
      				//	int pred;
      				//	coder.decode_int(&pred, q_r_G);
      					// Get the pixel
      				//	int pixel;
      					// Ill use the prediction just to see if works

      					// Write pixel
      				//	img_out.at<cv::Vec3b>(i,j)[1] = pixel;

      					// Increment the channel count
      				//	channel_counter++;

      					// Read a new bit
					//	bit = bitStream.getBit();

      				//}// End of the channel GREEN

      				//else{
      					// Quociente String
      				//	std::string q_B;

      					// Read quocient
      				//	while(q_flag == 0){

      				//		if(bit == 0){
	      			//			q_flag = 1;
	      			//			q_B += zero_num;
	      			//			break;
      				//		}
      				//		else if(bit == 1){
      				//			q_B += one_num;
      				//		}

      				//		bit = bitStream.getBit();
      				//	}

      					// Read reminder
      					// Since bitStream reads integers, we need a vector
      				//	std::vector<int> r_B_int;
      				//	int r_B_nBits = 11 - r_B_int.size();
      				//	r_B_int.resize(r_B_nBits);

      					// Read the amount of bits
      				//	bitStream.getNBit(r_B_int.data(), r_B_nBits);

      					//Create the string of reminder
      				//	std::string r_B;
      				//	for (int j = 0; j < r_B_nBits; j++){
      				//		if(r_B[j] == 1){
      				//			r_B += one_num;
      				//		}
      				//		else if(r_B[j] == 0){
      				//			r_B += zero_num;
      				//		}
      				//	}

      					// Concatenare q and r
      				//	std::string q_r_B = q_B + r_B;

      					// Make the decodation
      				//	int pred;
      				//	coder.decode_int(&pred, q_r_B);
      					// Get the pixel
      				//	int pixel;
      					// Ill use the prediction just to see if works

      					// Write pixel
      				//	img_out.at<cv::Vec3b>(i,j)[2] = pixel;
                              // 
      				//	// Reset because now we will read red next
      				//	channel_counter = 0;
                                        
      					// Read a new bit
					//	bit = bitStream.getBit();
      				//}// End of the channel BLUE
		      	}
		   	}
		//}
		// Write in to the file
		imwrite(outFile, img_out);
	}
};

#endif