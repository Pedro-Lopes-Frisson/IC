//
// Created by whoknows on 10/31/22.
//
#include "GolombCoding.h"
#include "BitStream.h"
#include <string>
#include <cmath>

using namespace std;

int main() {
  GolombCoder n = GolombCoder(int(pow(2, 16)));
  //GolombCoder n = GolombCoder((int) pow(2, 16));
  string bits;
  int og_nums[12] = {1, -1, 2, -2, 3, -3, -4, 5, -5, -6, 6, 0};
  int num_dec[12];
  for (size_t i = 0; i < 12; i++) {
    cout << "Num " << og_nums[i];
    n.encode_int(og_nums[i], bits);
    
    cout << "\tGolombCode: " << bits << endl;
    n.decode_int(&(num_dec[i]), bits);
  }
  
  for (size_t i = 0; i < 12; i++) {
    cout << "Num " << og_nums[i] << " Decoded: " << num_dec[i] << endl;
  }
  n.decode_int(&(num_dec[0]), "10 0000 0000 0000 0001");
  std::cout << "NMum: " << num_dec[0] << std::endl;
  
  return 0;
}
