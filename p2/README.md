# Code execution
Usage: ./build/p2/Decoder <encoded_file> <outfile>
Usage: ./build/p2/Encoder <GolombParameter> <block_size> <encoded_file> <outfile>
Usage: ./build/p2/GolombCoding
Usage: ./build/p2/LossyAudioCodec2 <GolombParameter> <block_size> <audio_file> <encoded audio file> <out audio file>
Usage: ./build/p2/exercicio1 <in file name> <out file name>
Usage: ./build/p2/exercicio2 <in file name> <Rotation Angle> <Value for Intensity>
Usage: ./build/p2/GolombCoding
Usage: ./build/p2/imgEncoder
Usage: ./build/p2/imgDecoder


# Folder Structure
```
..
└── p2
    └── dataset
        ├── audio
        ├── enc_files
        ├── dec_files
        └── images
```

1. p2 folder containing source code
2. p2/dataset contains audio files, image files, encoded files, decoded files.

# Scripts for creating CSV and Graphs, and also compile information related to the developed code

- createDataCsv.py
- compressed_ratio_graphs.py
- exetime_bs.py
- stat (linux utility)
- file_sizes.sh
- run_encoder.sh
- run_decoder.sh
