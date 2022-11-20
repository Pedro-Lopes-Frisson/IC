#include <fstream>
#include "BitStream.h"

using namespace std;

//00100101
int main() {
  //Encoder
  ifstream fIn("pedro_bin.txt"); // file with 1's and 0's
  BitStream b = BitStream("pedro.txt", BT_WRITE);
  char c;
  while (fIn.read(&c, sizeof(char))) {
    b.writeBit(c == '1');
  }
  b.close();
  fIn.close();
  
  //Decoder
  b.open("pedro.txt", BT_READ);
  ofstream fOut("out_file");
  auto bit = b.getBit();
  while (bit != EOF) {
    //std::cout << bit << endl;
    fOut << bit;
    bit = b.getBit();
  }
  fOut.close();
  b.close();
  
  // pe
  int arr[16] = {0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1};
  b.open("NBits.txt", BT_WRITE);
  b.writeNBit(arr, 16);
  b.close();
  
  // pd porque o padding que leva e de zeros
  int arr1[15] = {0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0};
  b.open("NBits2.txt", BT_WRITE);
  b.writeNBit(arr1, 15);
  b.close();
  
  
  
  return 0;
}
