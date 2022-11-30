#include "imgDecoder.h"
#include <string>

int main(int argc, char* argv[]){
  imgDecoder decoder {};
  std::string inFile = "lena_encoded";
  std::string outFile = "lena_decoded.png";
  decoder.decode_coded_image(inFile, outFile);
}