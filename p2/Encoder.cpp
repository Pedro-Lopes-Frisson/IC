#include "Encoder.h"
#include <string>

int main(int argc, char *argv[]) {
  long long int m;
  m = strtol(argv[1], NULL, 10);
  
  if (argc != 5) {
    std::cerr << "Usage: " << argv[0] << " <GolombParameter> <block_size> <encoded_file> <outfile>" << std::endl;
    return -1;
  }
  if (m < 0 || m > 16) {
    std::cerr << "Invalid Golomb Parameter 0-16" << std::endl;
    return -1;
  }
  
  long long int bs;
  bs = strtol(argv[2], NULL, 10);
  
  if (bs < 0) {
    std::cerr << "block_size can't be smaller than 0" << std::endl;
    return -1;
  }
  
  Encoder encoder{m, bs};
  std::string inFile = std::string(argv[argc - 2]);
  std::string outFile = std::string(argv[argc - 1]);
  encoder.encode_audio_file(inFile, outFile);
}
