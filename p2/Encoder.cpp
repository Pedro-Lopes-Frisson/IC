#include "Encoder.h"
#include <string>

int main(int argc, char* argv[]){
  long long int m;
  m = strtol(argv[1],NULL,10);
  if (m < 0 || m > 16){
    std::cerr << "Golomb Parameter to large" << std::endl;
  }
  Encoder encoder {m, 1024};
  std::string inFile = std::string(argv[argc-2]);
  std::string outFile =  std::string(argv[argc-1]);
  encoder.encode_audio_file(inFile, outFile);
}
