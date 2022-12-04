import matplotlib
import pandas as pd
import matplotlib.pyplot as plt
matplotlib.interactive(True)
df = pd.read_csv("fsize.csv")
plt.ion()



n = df.groupby(["OriginalFile", "BlockSize"], as_index=False)

#Create list Of Values of avg enc times for each blocksize per file and then plot
enc_times={}
dec_times={}
for i, (key,group) in enumerate(n):
    enc_times[key[0]] =[]
    dec_times[key[0]] =[]

for i, (key,group) in enumerate(n):
    enc_times[key[0]] += [(key[1],group["EncodingTime"].mean())]
    dec_times[key[0]] += [(key[1],group["DecodingTime"].mean())]

for k, v in dec_times.items():
    fig = plt.figure()
    plt.title(str(k))
    plt.plot([val[0] for val in v], [val[1] for val in v], marker='o')
    enc_time=enc_times[k]
    plt.plot([val[0] for val in enc_time], [val[1] for val in enc_time], marker='o')
    plt.xlabel('BlockSize');plt.ylabel('time')
    plt.legend(["Decode", "Encode"])
    fig.savefig("./graphs/"+str(k)+"_times.png", format="PNG")
