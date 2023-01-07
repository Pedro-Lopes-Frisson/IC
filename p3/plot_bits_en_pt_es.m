data2 = dlmread('values_bits_en_pt_es', ' ', 0,0);
x = [0:1:length(data2)-1];

hold on

plot (x, data2(:,1), 'or');
plot (x, data2(:,2), '^b');
plot (x, data2(:,3), 'xk');

xlabel "Position"
ylabel "Bits needed"
legend 'Ingles' 'Portugues' 'Espanhol'
