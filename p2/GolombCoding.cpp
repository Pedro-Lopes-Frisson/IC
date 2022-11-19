//
// Created by whoknows on 10/31/22.
//
#include "GolombCoding.h"
#include <string>
using namespace std;

int main() {
  //GolombCoder n = GolombCoder(10);
  GolombCoder n = GolombCoder((int) pow(2, 16));
  string bits;
  int og_nums[11] = {21,15,-1,2,1,-5,-7,5, 0, -1, 3};
  int num_dec[11];
  for(size_t i = 0; i < 11; i++){
    cout << "Num " << og_nums[i];
    n.encode_int(og_nums[i], bits);
  
    cout << "\tGolombCode: " << bits << endl;
    n.decode_int( &(num_dec[i]),bits);
  
  }
  
  for(size_t i = 0; i < 11; i++){
    cout << "Num " << og_nums[i] << " Decoded: " << num_dec[i] << endl;
  }

  return 0;
}
