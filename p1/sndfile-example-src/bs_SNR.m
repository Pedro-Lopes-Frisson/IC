set (0, "defaultaxesfontname", "Helvetica") % this is the line to add BEFORE plotting
SNR_sample = load("SNR_bs_frac")

figure
hold on

title("SNR values in relation to fraction of kept frequencies (lowest frequencies)")
for i = [1,6,11,16],
  plot(SNR_sample(i:i+3,2),SNR_sample(i:i+3,3), '-o')

  SNR_sample(i:i+4, 2:end)
end;
xlabel("Fraction of kept low frequencies")
ylabel("Signal to Noise Ratio(db)")
legend("Block Size 128","Block Size 256","Block Size 512","Block Size 1024")
print -djpg SNR_bsfrac.jpg
