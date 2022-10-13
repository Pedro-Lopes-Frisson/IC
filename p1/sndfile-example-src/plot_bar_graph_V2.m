clear all;
close all;
clc;

%B = dlmread('_quant_MID.txt', '	');
%bar(B(140:1:155,1),B(140:1:155,2))

quant_MID = readmatrix('_quant_MID.txt');
quant_SIDE = readmatrix('_quant_SIDE.txt');

samples_MID = quant_MID(:,1);
occ_MID = quant_MID(:,2);

samples_SIDE = quant_SIDE(:,1);
occ_SIDE = quant_SIDE(:,2);

figure(1)
bar(samples_MID,occ_MID)
title('Quantização MID')
xlabel('Samples')
ylabel('Ocorrências')


figure(2)
bar(samples_SIDE,occ_SIDE,'r')
title('Quantização SIDE')
xlabel('Samples')
ylabel('Ocorrências')
