#include "Decoder.h"
#include <string>

int main(int argc, char* argv[]){
  Decoder decoder {};
  std::string inFile = std::string(argv[argc-2]);
  std::string outFile =  std::string(argv[argc-1]);
  decoder.decode_Encoded_audio_file(inFile, outFile);
}