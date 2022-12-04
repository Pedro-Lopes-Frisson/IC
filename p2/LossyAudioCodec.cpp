//
// Created by whoknows on 11/30/22.
//

#include "LossyAudioCodec.h"

int main(int argc, char *argv[]) {
  if (argc != 8) {
    std::cerr << "Usage: " << argv[0]
              << " <GolombParameter> <block_size> <dct_bs> <dct_frac> <audio_file> <encoded audio file> <out audio file>"
              << std::endl;
    return -1;
  }

  long long int m;
  m = strtol(argv[1], NULL, 10);

  if (m < 0 || m > 16) {
    std::cerr << "Invalid Golomb Parameter 0-16" << std::endl;
    return -1;
  }

  long long int bs;
  bs = strtol(argv[2], NULL, 10);

  if (bs < 0 || bs > 4096) {
    std::cerr << "block_size can't be smaller than 0" << std::endl;
    return -1;
  }
  long long int dct_bs;
  dct_bs = strtol(argv[3], NULL, 10);

  if (dct_bs < 0 || dct_bs > 4096) {
    std::cerr << "DCT Block Size too big" << std::endl;
    return -1;
  }

  double dct_frac;
  dct_frac = strtod(argv[4], NULL);

  if (dct_frac <= 0 || dct_frac > 1) {
    std::cerr << "It mus be a value in this interval ]0,1]" << std::endl;
    return -1;
  }

  LossyAudioCodec codec{m, bs, 1024, 0.2};
  std::string inFile = std::string(argv[argc - 3]);
  std::string encFile = std::string(argv[argc - 2]);
  std::string outFile = std::string(argv[argc - 1]);

  codec.encode(inFile, encFile);

  codec.decode_audio_file(encFile, outFile);
}
