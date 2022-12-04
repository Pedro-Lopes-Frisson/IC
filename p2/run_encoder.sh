#!/bin/sh
blocksizes=(1024 2048 4096)
sizes=(11 12 13 14)
audiofile=( $(ls ./dataset/audio) )
out_path="./dataset/enc_files"

for ((f=0; f < ${#audiofile[@]}; f++)); do
	filename=$( echo ${audiofile[$f]} | sed 's/\..*//g') # filename no extension
	name=${audiofile[$f]} #full name
	for i in ${sizes[@]};do
		for bs in ${blocksizes[@]};do
			echo ./build/p2/Encoder $i $bs ./dataset/audio/$name $out_path"/"$filename"_enc_k_"$i"_bs_"$bs
			./build/p2/Encoder $i $bs ./dataset/audio/$name $out_path"/"$filename"_enc_k_"$i"_bs_"$bs > /dev/null &
		done;
	done;
done;
