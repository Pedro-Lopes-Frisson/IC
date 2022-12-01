#ifndef P2_IMGENCODER_H
#define P2_IMGENCODER_H

#include "BitStream.h"
#include "GolombCoding.h"
#include <string>
#include <vector>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <string.h>


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

	int prediction_residual(int pixel, int prediction){
		return pixel-prediction;
	}

	void JPEG_LS_predictor(cv::Mat img_in, cv::Mat &erro){
		cv::Size s = img_in.size(); 
		for(int i = 0; i < s.height; i++){
        	for(int j = 0; j < s.width; j++){
				int a,b,c;

				int pixel = img_in.at<uchar>(i,j);

				if(j == 0 && i != 0){
					a = 0;
					b = img_in.at<uchar>(i-1,j);
					c = 0;
				}

				else if(j != 0 && i == 0){
					a = img_in.at<uchar>(i,j-1);
					b = 0;
					c = 0;
				}

				else if(j == 0 && i == 0){
					a = 0;
					b = 0;
					c = 0;
				}

				else{
					a = img_in.at<uchar>(i,j-1);
					b = img_in.at<uchar>(i-1,j);
					c = img_in.at<uchar>(i-1,j-1);
				}

				int prediction = predict(a,b,c);

				int error = prediction_residual(pixel, prediction);

				erro.at<uchar>(i,j) = error;
			}
		}
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

		cv::Mat erro_R = cv::Mat::zeros(s.height, s.width, CV_8UC1);
		cv::Mat erro_G = cv::Mat::zeros(s.height, s.width, CV_8UC1);
		cv::Mat erro_B = cv::Mat::zeros(s.height, s.width, CV_8UC1);
		cv::Mat img_in_R = cv::Mat::zeros(s.height, s.width, CV_8UC1);
		cv::Mat img_in_G = cv::Mat::zeros(s.height, s.width, CV_8UC1);
		cv::Mat img_in_B = cv::Mat::zeros(s.height, s.width, CV_8UC1);

		for(int i = 0; i < s.height; i++){
        	for(int j = 0; j < s.width; j++){
				img_in_R.at<uchar>(i,j) = img_in.at<cv::Vec3b>(i,j)[2];
				img_in_G.at<uchar>(i,j) = img_in.at<cv::Vec3b>(i,j)[1];
				img_in_B.at<uchar>(i,j) = img_in.at<cv::Vec3b>(i,j)[0];
			}
		}		
		
		// Encode the image size
		//std::string bits_Height;
		//std::string bits_Width;
		//coder.encode_int(); // Height
		//coder.encode_int(); // Width

		JPEG_LS_predictor(img_in_R, erro_R);
		JPEG_LS_predictor(img_in_G, erro_G);
		JPEG_LS_predictor(img_in_B, erro_B);

		//std::cout << img_in_B.at<uchar>(0,0) << std::endl;
		// Run through every pixel of the image
		for(int i = 0; i < s.height; i++){
        	for(int j = 0; j < s.width; j++){
				std::string bits_R;
				std::string bits_G;
				std::string bits_B;

				// Encode blue channel
				coder.encode_int((int)erro_B.at<uchar>(i,j), bits_B);
				write_to_file(bits_B, bitStream);
				// Encode green channel
				coder.encode_int((int)erro_G.at<uchar>(i,j), bits_G);
				write_to_file(bits_G, bitStream);
				// Encode red channel
				coder.encode_int((int)erro_R.at<uchar>(i,j), bits_R);
				write_to_file(bits_R, bitStream);
        	}
    	}
    	bitStream.close();
    }
};
#endif