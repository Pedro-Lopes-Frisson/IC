# Usage
Usage: ../sndfile-example-bin/DCTDecoder number_of_channels coded_wav wavOut

Usage: ../sndfile-example-bin/DCTEncoder [ -v (verbose) ] [ -bs blockSize (def 1024) ] [ -frac dctFraction (def 0.2) ] wavFileIn  encoded file out

Usage: ../sndfile-example-bin/wav_cmp <original file> <other file>

Usage: wav_cp [ -v (verbose) ] wavFileIn wavFileOut

Usage: wav_dct [ -v (verbose) ] [ -bs blockSize (def 1024) ] [ -frac dctFraction (def 0.2) ] wavFileIn wavFileOut

Usage: ../sndfile-example-bin/wav_hist <input file> <channel>

Usage ./wav_quant <input-file> <output-file> where output file is a quantized version of input files

Usage ./wav_effects Assuming that you have a file called sample.wav at the current working dir. This will create 2 new files one with it's amplitude reduced and another on with eco

Usage ./bitStream creates encoded files and decodes them into 0's and 1's
