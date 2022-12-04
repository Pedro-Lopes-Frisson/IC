#include "Decoder.h"
#include <string>

int main(int argc, char* argv[]){
  if(argc != 3){
    std::cerr << "Usage: " << argv[0] << " <encoded_file> <outfile>" << std::endl;
  }
  Decoder decoder {0, 0};
  std::string inFile = std::string(argv[argc-2]);
  std::string outFile =  std::string(argv[argc-1]);
  decoder.decode_Encoded_audio_file(inFile, outFile);
}