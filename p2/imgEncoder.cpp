#include "imgEncoder.h"
#include <string>

int main(int argc, char* argv[]){
  imgEncoder encoder {};
  std::string inFile = "monarch.ppm";
  std::string outFile = "lena_encoded";
  encoder.encode_image(inFile, outFile);
}
