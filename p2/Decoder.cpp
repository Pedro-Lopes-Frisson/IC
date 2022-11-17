#include "Dencoder.h"
#include <string>

int main(int argc, char* argv[]){
  Decoder decoder {};
  std::string inFile = "sample01_codedenc";
  std::string outFile = "./sample01_decoded.wav";
  decoder.decode_Encoded_audio_file(inFile, outFile);
}