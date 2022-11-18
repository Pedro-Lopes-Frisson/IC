//
// Created by whoknows on 11/18/22.
//

#ifndef P2_BITSTREAM_H
#define P2_BITSTREAM_H

/*
 * Openning modes
 */
#include <fstream>
#include <iostream>
typedef enum
{
  BT_READ = 0,
  BT_WRITE = 1,
  BT_NO_MODE
} BT_MODES;


class BitStream {
private:
  std::ifstream *inStream;
  std::ofstream *outStream;
  char bit_buffer;
  short bit_count;
  BT_MODES modes;
  
public:
  BitStream(void);
  BitStream(const char* filename, const BT_MODES mode);
  virtual ~BitStream(void);
  
  void open(const char* filename, const BT_MODES mode);
  void close(void);
  
  int getBit(void);
  int writeBit(const int b);
  
  int getNBit(int *store_bits, size_t size);
  int writeNBit(int *write_bits, size_t size);
  

};


#endif //P2_BITSTREAM_H
