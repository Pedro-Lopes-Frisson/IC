#include "Decoder.h"
#include <string>

int main(int argc, char* argv[]){
  if(argc != 4){
    std::cerr << "Usage: " << argv[0] << "<block_size> <encoded_file> <outfile>" << std::endl;
  }
  int bs;
  bs = strtol(argv[1],NULL,10);
  if (bs < 0){
    std::cerr << "block_size can't be smaller than 0" << std::endl;
    return -1;
  }
  Decoder decoder {0, bs};
  std::string inFile = std::string(argv[argc-2]);
  std::string outFile =  std::string(argv[argc-1]);
  decoder.decode_Encoded_audio_file(inFile, outFile);
}