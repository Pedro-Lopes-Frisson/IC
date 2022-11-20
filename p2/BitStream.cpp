//
// Created by whoknows on 11/18/22.
//

#include "BitStream.h"

using namespace std;

BitStream::BitStream(void) {
  inStream = NULL;
  outStream = NULL;
  bit_buffer = 0;
  bit_count = 0;
  modes = BT_NO_MODE;
}

BitStream::BitStream(const char *filename, const BT_MODES mode) {
  inStream = NULL;
  outStream = NULL;
  bit_buffer = 0;
  bit_count = 0;
  switch (mode) {
    case BT_READ:
      inStream = new ifstream(filename, ios::in | ios::binary);
      if (!inStream->good()) {
        delete inStream;
        inStream = NULL;
      } else {
        modes = mode;
      }
      break;
    case BT_WRITE:
      outStream = new ofstream(filename, ios::out | ios::binary);
      if (!outStream->good()) {
        delete outStream;
        outStream = NULL;
      } else {
        modes = mode;
      }
      break;
    default:
      throw ("Invalid File Openning mode");
      break;
    
  }
  /* make sure we opened a file */
  if ((inStream == NULL) && (outStream == NULL)) {
    throw ("Unable To Open File");
  }
  
  
}

BitStream::~BitStream(void) {
  if (inStream != NULL) {
    inStream->close();
  }
  if (outStream != NULL) {
    if (bit_count != 0) {
      bit_buffer <<= (8 - bit_count);
      outStream->put(bit_buffer);
    }
    
    outStream->close();
    delete outStream;
  }
}

void BitStream::open(const char *filename, const BT_MODES mode) {
  if ((inStream != NULL) || (outStream != NULL)) {
    throw ("There is a file already open");
  }
  
  switch (mode) {
    case BT_READ:
      inStream = new ifstream(filename, ios::in | ios::binary);
      if (!inStream->good()) {
        delete inStream;
        inStream = NULL;
      } else {
        modes = mode;
      }
      break;
    case BT_WRITE:
      outStream = new ofstream(filename, ios::out | ios::binary);
      if (!outStream->good()) {
        delete outStream;
        outStream = NULL;
      } else {
        modes = mode;
      }
      bit_count = 0;
      bit_buffer = 0;
      break;
    
    default:
      throw ("Invalid File Openning mode");
      break;
    
  }
  /* make sure we opened a file */
  if ((inStream == NULL) && (outStream == NULL)) {
    throw ("Unable To Open File");
  }
}

void BitStream::close() {
  if (inStream != NULL) {
    inStream->close();
    delete inStream;
    inStream = NULL;
    bit_buffer = 0;
    bit_count = 0;
    modes = BT_NO_MODE;
  }
  if (outStream != NULL) {
    if (bit_count != 0) {
      //add 0's
      bit_buffer <<= (8 - bit_count);
      //write
      outStream->put(bit_buffer);
    }
    // close stream reset variables
    outStream->close();
    delete outStream;
    outStream = NULL;
    bit_buffer = 0;
    bit_count = 0;
    modes = BT_NO_MODE;
  }
}

int BitStream::getBit() {
  int returnValue;
  
  if (inStream == NULL) {
    return EOF;
  }
  
  if (bit_count == 0) {
    /* buffer is empty, read another character */
    if ((returnValue = inStream->get()) == EOF) {
      return EOF;         /* nothing left to read */
    } else {
      bit_count = 8;
      bit_buffer = returnValue;
    }
  }
  
  /* bit to return is msb in buffer */
  bit_count--;
  // this leaves the righmost values has the one bit that we are currently reading
  returnValue = bit_buffer >> bit_count;
  
  // obtains the bit value by applying a mask
  return (returnValue & 0x01);
}

int BitStream::writeBit(const int b) {
  int returnValue = b;
  
  if (outStream == NULL) {
    return EOF;
  }
  
  // increment bit_count
  bit_count++;
  // shift left insert a bit to the right
  bit_buffer <<= 1;
  
  if (b != 0) {
    // or makes the right most bit at 1
    bit_buffer |= 1;
  }
  
  /* write bit buffer if we have 8 bits */
  if (bit_count == 8) {
    outStream->put(bit_buffer);    /* check for error */
    
    /* reset buffer */
    bit_count = 0;
    bit_buffer = 0;
  }
  
  return returnValue;
}

int BitStream::writeNBit(int *write_bits, size_t size) {
  for (size_t i = 0; i < size; i++) {
    this->writeBit(write_bits[i]);
  }
  return 1;
}

int BitStream::getNBit(int *store_bits, size_t size) {
  for (size_t i = 0; i < size; i++) {
    store_bits[i] = this->getBit();
  }
  return 1;
}
