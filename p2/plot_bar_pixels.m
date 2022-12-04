close all;
clear all;
clc;

% Pixeis Red channel
P_1024_R = readmatrix('R_pixel_enc_1024');
% Residuos Red channel
R_1024_R = readmatrix('R_res_enc_1024');


% Pixeis Green channel
P_1024_G = readmatrix('G_pixel_enc_1024');
% Residuos Green channel
R_1024_G = readmatrix('G_res_enc_1024');

% Pixeis Blue channel
P_1024_B = readmatrix('B_pixel_enc_1024');
% Residuos Blue channel
R_1024_B = readmatrix('B_res_enc_1024');

figure(1)
subplot(1, 2, 1);
title('Imagem - Monarch.ppm - Block Size 1024');
histogram(P_1024_R, 'FaceColor','r');
xlabel('Pixeis Canal Vermelho');
ylabel('Ocorrências');
subplot(1, 2, 2);
title('Imagem - Monarch.ppm - Block Size 1024');
histogram(R_1024_R, 'FaceColor','r');
xlabel('Resíduos Canal Vermelho');
ylabel('Ocorrências');


figure(2)
subplot(1, 2, 1);
title('Imagem - Monarch.ppm - Block Size 1024');
histogram(P_1024_G, 'FaceColor','g');
xlabel('Pixeis Canal Verde');
ylabel('Ocorrências');
subplot(1, 2, 2);
title('Imagem - Monarch.ppm - Block Size 1024');
histogram(R_1024_G, 'FaceColor','g');
xlabel('Resíduos Canal Verde');
ylabel('Ocorrências');


figure(3)
subplot(1, 2, 1);
title('Imagem - Monarch.ppm - Block Size 1024');
histogram(P_1024_B, 'FaceColor','b');
xlabel('Pixeis Canal Azul');
ylabel('Ocorrências');
subplot(1, 2, 2);
title('Imagem - Monarch.ppm - Block Size 1024');
histogram(R_1024_B, 'FaceColor','b');
xlabel('Resíduos Canal Azul');
ylabel('Ocorrências');
