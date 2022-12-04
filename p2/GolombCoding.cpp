//
// Created by whoknows on 10/31/22.
//
#include "GolombCoding.h"
#include "BitStream.h"
#include <string>
#include <cmath>

using namespace std;

int main() {
  GolombCoder n = GolombCoder(10);
  //GolombCoder n = GolombCoder((int) pow(2, 16));
  string bits;
  //int og_nums[12] = {1, -1, 2, -2, 3, -3, -4, 5, -5, -6, 6, 0};
  long long int num_dec;
  for (long long int  i = 0; i < 10; i++) {
    cout << "Num " << i;
    n.encode_int(i, bits);
    
    cout << "\tGolombCode: " << bits;
    n.decode_int(&num_dec, bits);
    cout << "\t Decoded Num: " << num_dec;
    cout << "\t Size " << bits.size() << endl;
    
  }
  
  
  return 0;
}
