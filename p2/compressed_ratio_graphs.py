import matplotlib
import pandas as pd
import matplotlib.pyplot as plt
matplotlib.interactive(True)
df = pd.read_csv("fsize.csv")
df["CompressedRatio"] = (df["OriginalSize"] - df["EncodedFileSize"]) / (df["OriginalSize"]) * 100
plt.ion()


df.set_index("CompressedRatio")

n = df.groupby("OriginalFile")

#fig, axes = plt.subplots(n.ngroups, sharex=True, figsize=(8, 6))

for i, (files,group) in enumerate(n):
    bs = group.groupby("BlockSize")
    fig = plt.figure()
    plt.title(files)
    for r, (bs,g_n) in enumerate(bs):
        print(bs)
        g_n.set_index("InitialGolombOrder")["CompressedRatio"].plot(ylabel='CompressedRatio', xlabel="InitialGolombOrder", marker='o',linestyle='-')
        print(g_n.set_index("InitialGolombOrder")["CompressedRatio"].head())
        plt.legend([ "Block size" + str(bs_s) for bs_s in set(df["BlockSize"]) ])
    fig.savefig("./graphs/"+files+".png", format="PNG")

plt.show(block=True)
