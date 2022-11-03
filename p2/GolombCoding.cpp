//
// Created by whoknows on 10/31/22.
//

#include "GolombCoding.h"
#include <string>
using namespace std;

int main() {
  GolombCoder n = GolombCoder(10);
  string bits;
  n.encode_int(-42, bits);
  int num;
  cout << bits << "\t";
  n.decode_int( &num,bits);
  cout << num << endl;
  
  n.encode_int(42, bits);
  cout << bits << "\t";
  n.decode_int( &num,bits);
  cout << num << endl;
  return 0;
}