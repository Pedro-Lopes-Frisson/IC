//
// Created by whoknows on 12/1/22.
//

#include "LossyAudioCodec2.h"

int main(int argc, char *argv[]) {
  if (argc != 7) {
    std::cerr << "Usage: " << argv[0]
              << " <GolombParameter> <block_size> <audio_file> <encoded audio file> <out audio file>"
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

  int q_f;
  q_f = strtol(argv[3], NULL, 10);

  if (q_f < 0 || q_f > 16) {
    std::cerr << "Quantization factor 0-16" << std::endl;
    return -1;
  }

  LossyAudioCodec2 codec(m, bs, q_f );
  std::string inFile = std::string(argv[argc - 3]);
  std::string encFile = std::string(argv[argc - 2]);
  std::string outFile = std::string(argv[argc - 1]);

  codec.encode_audio_file(inFile, encFile);
  std::cout  << "Decode \n";
  codec.decode_Encoded_audio_file(encFile, outFile);
}
