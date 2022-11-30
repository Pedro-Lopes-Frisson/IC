#ifndef P2_IMGENCODER_H
#define P2_IMGENCODER_H

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


class imgEncoder{
private:
	// Defining the Golomb
	GolombCoder coder{(int) pow(2, 10)};

	// Functions
	int predict(int a, int b, int c) {

		int p;
		if(c >= std::max(a, b)){
			p = std::min(a, b);
		}
		else if(c <= std::min(a, b)) {
			p = std::max(a, b);
		}
		else{
			p = a + b - c;
		}

    	return p;
  	}

  	void write_to_file(std::string bits, BitStream &bitStream) {
    	for (size_t i = 0; i < bits.size(); i++) {
      		bitStream.writeBit(bits[i] == '1');
    	}
  	}

public:

	// Inicialize the class to encode
	imgEncoder() {

	}

	// Function to encode the image
	void encode_image(std::string inFile, std::string outFile) {

		// Convert the string path of image into an image
		cv::Mat img_in = cv::imread(inFile);

		// Verify the image
		if(img_in.empty()) {
			std::cout << "\033[1;31mError: Image provided does not exist!\033[0m" << std::endl;
		}

		// Open the BitSream to write to file
		BitStream bitStream = BitStream(outFile.data(), BT_WRITE);
		// Inicialize / defining all variables needed
		// number os pixels of the image
		cv::Size s = img_in.size(); 

		// Encode the image size
		//std::string bits_Height;
		//std::string bits_Width;
		//coder.encode_int(); // Height
		//coder.encode_int(); // Width

		// Run through every pixel of the image
		for(int i = 0; i < s.height; i++){
        	for(int j = 0; j < s.width; j++){

        		// For the first line we cant predict
        		if(i == 0 || j == 0){

                    // Bits to write on every channel
                    std::string bits_R;
                    std::string bits_G;
                    std::string bits_B;

        			// Encode red channer
        			coder.encode_int((int)img_in.at<cv::Vec3b>(i,j)[0], bits_R);
                    write_to_file(bits_R, bitStream);
        			// Encode green channer
        			coder.encode_int((int)img_in.at<cv::Vec3b>(i,j)[1], bits_G);
                    write_to_file(bits_G, bitStream);
        			// Encode blue channer
        			coder.encode_int((int)img_in.at<cv::Vec3b>(i,j)[2], bits_B);
                    write_to_file(bits_B, bitStream);

        		}
        		// Nas restantes já é possível
        		else{

                    // Bits to write on every channel
                    std::string bits_R;
                    std::string bits_G;
                    std::string bits_B;

        			// abc for red channel
        			int a_R = (int)img_in.at<cv::Vec3b>(i,j-1)[0];
        			int b_R = (int)img_in.at<cv::Vec3b>(i-1,j)[0];
        			int c_R = (int)img_in.at<cv::Vec3b>(i-1,j-1)[0];
        			// Encode the prediction for red channel
        			coder.encode_int(predict(a_R, b_R, c_R), bits_R);
        			write_to_file(bits_R, bitStream);

        			// abc for green channel
        			int a_G = (int)img_in.at<cv::Vec3b>(i,j-1)[1];
        			int b_G = (int)img_in.at<cv::Vec3b>(i-1,j)[1];
        			int c_G = (int)img_in.at<cv::Vec3b>(i-1,j-1)[1];
        			// Encode the prediction for green channel
        			coder.encode_int(predict(a_G, b_G, c_G), bits_G);
        			write_to_file(bits_G, bitStream);

        			// abc for blue channel
        			int a_B = (int)img_in.at<cv::Vec3b>(i,j-1)[2];
        			int b_B = (int)img_in.at<cv::Vec3b>(i-1,j)[2];
        			int c_B = (int)img_in.at<cv::Vec3b>(i-1,j-1)[2];
        			// Encode the prediction for green channel
        			coder.encode_int(predict(a_B, b_B, c_B), bits_B);
        			write_to_file(bits_B, bitStream);

        		}

        	}
    	}
    	bitStream.close();
    }
};
#endif