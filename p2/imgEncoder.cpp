#include "imgEncoder.h"
#include <string>

int main(int argc, char* argv[]){
  imgEncoder encoder {};
  std::string inFile = "./dataset/images/anemone.ppm";
  std::string outFile = "lena_encoded";
  encoder.encode_image(inFile, outFile);
}
