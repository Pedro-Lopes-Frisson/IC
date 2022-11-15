//
// Created by whoknows on 11/14/22.
//

#include "Encoder.h"
#include <string>

int main(int argc, char* argv[]){
  Encoder encoder {};
  std::string inFile = "./../p1/dataset/sample01.wav";
  std::string outFile = "./sample01";
  encoder.encode_audio_file(inFile, outFile);
}

