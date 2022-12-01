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
		auto bit = bitStream.getBit();

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
      	cv::Mat img_out = cv::Mat::zeros(cv::Size(512,512), CV_8UC1);

		// Run through every bit in the coded file
		while(bit != EOF){

      		// For loop to get the pixels in the right position
      		for(int i = 0; i < 512; i++){
      			for(int j = 0; j < 512; j++){
					for(int k = 0; k < 3; k++){
						std::string quocient;

      					// Read quocient
      					while(q_flag == 0){
      						if(bit == 0){
	      						q_flag = 1;
	      						quocient += zero_num;
	      						break;
      						}
      						else if(bit == 1){
      							quocient += one_num;
      						}
      						bit = bitStream.getBit();
      					}
						q_flag = 0;
						//std::cout << "GETS HERE" << std::endl;
      					// Read reminder
      					// Since bitStream reads integers, we need a vector
      					std::vector<int> r_R_int;
      					int r_R_nBits = 11 - r_R_int.size();
						//std::cout << r_R_nBits << std::endl;
      					r_R_int.resize(r_R_nBits);

      					// Read the amount of bits
      					bitStream.getNBit(r_R_int.data(), r_R_nBits);
						//for(int z = 0; z < 11; z++)
							//std::cout << r_R_int[z] << std::endl;
      					//Create the string of reminder
      					std::string remainder;
      					for (int x = 0; x < r_R_nBits; x++){
      						if(r_R_int[x] == 1){
      							remainder += one_num;
      						}
      						else if(r_R_int[x] == 0){
      							remainder += zero_num;
      						}
							//std::cout << remainder << std::endl;
      					}

      					// Concatenare q and r
      					std::string q_r = quocient + remainder;
      					// Make the decodation
      					long long int pred;
      					coder.decode_int(&pred, q_r);
      					// Get the pixel
      					//int pixel;
      					// Ill use the prediction just to see if works
						//std::cout << img_out.at<cv::Vec3b>(i,j) << std::endl;
						//std::cout << pred << std::endl;
      					// Write pixel
						std::cout << pred << std::endl;
      					img_out.at<uchar>(i,j) = pred;
						q_r.clear();
					}	
				}
		   	}
			bit = bitStream.getBit();
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