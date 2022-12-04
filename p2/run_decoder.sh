#!/bin/sh
in_path="./dataset/enc_files"
out_path="./dataset/dec_files"
audiofile=( $(ls $in_path) )

for ((f=0; f < ${#audiofile[@]}; f++)); do
	filename=$( echo ${audiofile[$f]} | sed 's/_enc_/_/g') ## filename without _enc_
	name=${audiofile[$f]}	# full name

	echo ./build/p2/Decoder $in_path"/"$name $out_path"/"$filename"_dec.wav"
	./build/p2/Decoder $in_path"/"$name $out_path"/"$filename"_dec.wav" > /dev/null &
done;
