#include "Decoder.h"
#include <string>

int main(int argc, char* argv[]){
  Decoder decoder {};
  std::string inFile = "sample07_coded";
  std::string outFile = "sample07_decoded.wav";
  decoder.decode_Encoded_audio_file(inFile, outFile);
}