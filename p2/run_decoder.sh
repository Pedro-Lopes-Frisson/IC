#!/bin/sh
sizes=(1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16)
in_path="./dataset/enc_files"
out_path="./dataset/dec_files"
audiofile=( $(ls $in_path) )

for ((f=0; f < ${#audiofile[@]}; f++)); do
	filename=$( echo ${audiofile[$f]} | sed 's/_enc_/_/g')
	name=${audiofile[$f]}
	n=$( echo ${audiofile[$f]} | sed 's/.*_enc_//g')
	echo  ./build/p2/Decoder $n $in_path"/"$name $out_path"/"$filename"_dec.wav"
	./build/p2/Decoder 1024 $in_path"/"$name $out_path"/"$filename"_dec.wav" > /dev/null &
done;
