#!/bin/sh
sizes=(11 12 13 14)
audiofile=( $(ls ./dataset/audio) )
out_path="./dataset/enc_files"

for ((f=0; f < ${#audiofile[@]}; f++)); do
	filename=$( echo ${audiofile[$f]} | sed 's/\..*//g')
	name=${audiofile[$f]}
	for i in ${sizes[@]};do
		echo ./build/p2/Encoder $i ./dataset/audio/$name $out_path"/"$filename"_enc_"$i
		./build/p2/Encoder $i 1024 ./dataset/audio/$name $out_path"/"$filename"_enc_"$i > /dev/null &
	done;
done;
