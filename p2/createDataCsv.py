audio_enc_file={}

with open("file_sizes") as f:
    lines = f.readlines()
    original_files=[file for file in lines if "wav" in file and "dec" not in file]
    for og_f in original_files:
        file=og_f.split("Size")[0].split("/")[3].replace(" ","")
        size=og_f.split("Size:")[1].replace(" ", "").rstrip()
        audio_enc_file[(file,size)] = set()

    for key in audio_enc_file.keys():
        f,_ = key
        f_name = f.split(".wav")[0]
        for entry in lines:
            e = entry.replace("\n", "")
            if "enc" in entry and f_name in entry:
                n=e.split("Size")[0].split("/")[3].replace(" ","")
                k=e.split("Size")[0].split("/")[3].replace(" ","").split("_")[-1]
                size=e.split("Size:")[1].replace(" ", "").rstrip()
                audio_enc_file[key].add((k, size))





# write csv

for k, v in audio_enc_file.items():
    for entry in v:
        print(k[0], k[1],entry[0], entry[1],sep=",",end="\n" )
