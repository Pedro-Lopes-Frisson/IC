#include "Encoder.h"
#include <string>

int main(int argc, char* argv[]){
  Encoder encoder {};
  std::string inFile = "sample07.wav";
  std::string outFile = "./sample07_coded";
  encoder.encode_audio_file(inFile, outFile);
}
