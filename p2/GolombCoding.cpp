//
// Created by whoknows on 10/31/22.
//
#include "GolombCoding.h"
#include "BitStream.h"
#include <string>
using namespace std;

int main() {
  GolombCoder n = GolombCoder(1000);
  string bits;
  int og_nums[11] = {10,0,23,34,45,56,67,89, -19, -22, -44};
  int num_dec[11];
  BitStream writeF {"GolombCode.txt", BT_WRITE};
  for(size_t i = 0; i < 11; i++){
    n.encode_int(i, bits);
    cout << "Num " << i ;
    cout << "\tGolombCode: " << bits << endl;
    n.decode_int( &(num_dec[i]),bits);
    for(char c : bits ){
      writeF.wr
    }
  }

  for(size_t i = 0; i < 11; i++){
    cout << "Num " << i << " Decoded: " << num_dec[i] << endl;
  }

  return 0;
}
