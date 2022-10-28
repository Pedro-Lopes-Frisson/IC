set (0, "defaultaxesfontname", "Helvetica") % this is the line to add BEFORE plotting
snr_sizes = load("SNR_Sizes")
figure
hold on
title("SNR Values in relation to encoded file size")
scatter(snr_sizes(:,1), snr_sizes(:,1));
xlabel("File size (bytes)")
ylabel("Signal to Noise Ratio")
print -djpg SNR_fsize.jpg
