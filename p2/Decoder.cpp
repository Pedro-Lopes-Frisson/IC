#include "Decoder.h"
#include <string>

int main(int argc, char* argv[]){
  int m;
  m = strtol(argv[1],NULL,10);
  Decoder decoder {m};
  std::string inFile = std::string(argv[argc-2]);
  std::string outFile =  std::string(argv[argc-1]);
  decoder.decode_Encoded_audio_file(inFile, outFile);
}