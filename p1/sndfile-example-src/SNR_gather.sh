sizes=(128 256 512 1024 2048)
frac=(0.2 0.4 0.6 0.8)

for s in ${sizes[@]};do
  for f in ${frac[@]};do
    echo $f " " $s
    # encode audio
    ../sndfile-example-bin/DCTEncoder -frac $f -bs $s sample.wav $(echo -n "sample_bs"$s"_frac"$f) &> /dev/null
    # decode audio
    ../sndfile-example-bin/DCTDecoder  $(echo -n "sample_bs"$s"_frac"$f"enc")  $(echo -n "sample_bs"$s"_frac"$f".wav")  &> /dev/null
    # get SNR
    echo $s" "$f" "$(../sndfile-example-bin/wav_cmp sample.wav $(echo -n "sample_bs"$s"_frac"$f".wav") | awk -F"SNR = " '{printf $2}' ) >> SNR_bs_frac
    # get encoded file size
    echo $s" "$f" "$( /bin/ls -l | grep $(echo -n "sample_bs"$s"_frac"$f"enc")  | awk -F " " '{printf $5}') >> bs_frac_fsize
    echo $(../sndfile-example-bin/wav_cmp sample.wav $(echo -n "sample_bs"$s"_frac"$f".wav") | awk -F"SNR = " '{printf $2}' )" "$( /bin/ls -l | grep $(echo -n "sample_bs"$s"_frac"$f"enc")  | awk -F " " '{printf $5}') >> SNR_size
  done
done
