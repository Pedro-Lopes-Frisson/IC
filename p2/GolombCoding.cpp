//
// Created by whoknows on 10/31/22.
//
#include "GolombCoding.h"
#include "BitStream.h"
#include <string>
using namespace std;

int main() {
  //GolombCoder n = GolombCoder(10);
  GolombCoder n = GolombCoder();
  string bits;
  int og_nums[11] = {0,12,23,34,45,56,67,89, -19, -22, -44};
  int num_dec[11];
  BitStream writeF {"GolombCode.txt", BT_WRITE};
  for(size_t i = 0; i < 11; i++){
    cout << "Num " << og_nums[i];
    n.encode_int(og_nums[i], bits);

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
