# Code
## Code Compilation
You need to install boost. Boost is the serialization library used to save and load models.

Some problems may arise so make sure you edit CMakeLists.txt according to your problems.

`make` will compile the code and every library as well as make sure everything is linked.

`make clean` eliminates everyfile created in the compilation process.


## Code Execution
```


./build/p3/fcm_test_novo'

./build/p3/lang_novo 6 0.90 mini_example_PT.txt dataset/Europarl/de_DE.German-latn-EP7.utf8

./build/p3/lang_novo 6 0.90 mini_example_PT.txt dataset/Europarl/de_DE.German-latn-EP7.utf8'


./build/p3/findlang_novo 3 0.5 mini_example_ENG.txt dataset/Europarl/de_DE.German-latn-EP7.utf8 dataset/Europarl/en_GB.English-latn-EP7.utf8 dataset/Europarl/es_ES.Spanish-latn-EP7.utf8 dataset/Europarl/et_EE.Estonian-latn-EP7.utf8 dataset/Europarl/fi_FI.Finnish-latn-EP7.utf8 dataset/Europarl/fr_FR.French-latn-EP7.utf8 dataset/Europarl/it_IT.Italian-latn-EP7.utf8 dataset/Europarl/nl_NL.Dutch-latn-EP7.utf8 dataset/Europarl/pt_PT.Portugese-latn-EP7.utf8 dataset/Europarl/ro_RO.Romanian-latn-EP7.utf8 

./build/p3/findlang_novo 6 0.5 mini_example_ENG.txt dataset/Europarl/de_DE.German-latn-EP7.utf8 dataset/Europarl/en_GB.English-latn-EP7.utf8 dataset/Europarl/es_ES.Spanish-latn-EP7.utf8 dataset/Europarl/et_EE.Estonian-latn-EP7.utf8 dataset/Europarl/fi_FI.Finnish-latn-EP7.utf8 dataset/Europarl/fr_FR.French-latn-EP7.utf8 dataset/Europarl/it_IT.Italian-latn-EP7.utf8 dataset/Europarl/nl_NL.Dutch-latn-EP7.utf8 dataset/Europarl/pt_PT.Portugese-latn-EP7.utf8 dataset/Europarl/ro_RO.Romanian-latn-EP7.utf8'


./build/p3/locatelang mini_example_ToBeAnalised_EN_with_PT.txt -k 3 -a  0.5 -b 10 -l mini_example_ENG.txtprob_table -l mini_example_ESP.txtprob_table'

./build/p3/locatelang mini_example_ToBeAnalised_EN_with_PT.txt -k 3 -a  0.5 -b 10 -r mini_example_ENG.txt -r mini_example_ESP.txt'
```

## Considerations

Models that are loaded must comply with parameters k and alpha inserted as arguments of the respective program.

This is basically a classification problem which means that languages that are very similar will sometimes collide.

## Execution Examples

### lang

```
Time to count ocorrences: 5.0141
Time to calculate probabilities: 2.894
Time to calculate nº bits: 0.010565
Using the dataset/Europarl/de_DE.German-latn-EP7.utf8 model, the under analise text would need 21400 bits to be writen.
```

### findlang

```
Time to count ocorrences: 5.35962
Time to calculate probabilities: 2.9773
Time to calculate nº bits: 0.01154
Using the dataset/Europarl/de_DE.German-latn-EP7.utf8 model, the under analise text would need 21591 bits to be writen.
Time to count ocorrences: 5.1407
Time to calculate probabilities: 3.1454
Time to calculate nº bits: 0.015008
Using the dataset/Europarl/en_GB.English-latn-EP7.utf8 model, the under analise text would need 14800 bits to be writen.
Time to count ocorrences: 4.9264
Time to calculate probabilities: 2.6428
Time to calculate nº bits: 0.022708
Using the dataset/Europarl/es_ES.Spanish-latn-EP7.utf8 model, the under analise text would need 21481 bits to be writen.
Time to count ocorrences: 5.6328
Time to calculate probabilities: 4.2502
Time to calculate nº bits: 0.012254
Using the dataset/Europarl/et_EE.Estonian-latn-EP7.utf8 model, the under analise text would need 21739 bits to be writen.
Time to count ocorrences: 5.2258
Time to calculate probabilities: 3.6454
Time to calculate nº bits: 0.010261
Using the dataset/Europarl/fi_FI.Finnish-latn-EP7.utf8 model, the under analise text would need 21571 bits to be writen.
Time to count ocorrences: 5.206
Time to calculate probabilities: 2.6574
Time to calculate nº bits: 0.011558
Using the dataset/Europarl/fr_FR.French-latn-EP7.utf8 model, the under analise text would need 21245 bits to be writen.
Time to count ocorrences: 5.575
Time to calculate probabilities: 2.7627
Time to calculate nº bits: 0.011919
Using the dataset/Europarl/it_IT.Italian-latn-EP7.utf8 model, the under analise text would need 21555 bits to be writen.
Time to count ocorrences: 5.625
Time to calculate probabilities: 3.0109
Time to calculate nº bits: 0.010893
Using the dataset/Europarl/nl_NL.Dutch-latn-EP7.utf8 model, the under analise text would need 21443 bits to be writen.
Time to count ocorrences: 5.2974
Time to calculate probabilities: 2.9373
Time to calculate nº bits: 0.011539
Using the dataset/Europarl/pt_PT.Portugese-latn-EP7.utf8 model, the under analise text would need 21493 bits to be writen.
Time to count ocorrences: 5.1314
Time to calculate probabilities: 3.5818
Time to calculate nº bits: 0.021087
Using the dataset/Europarl/ro_RO.Romanian-latn-EP7.utf8 model, the under analise text would need 21371 bits to be writen.


The language of the text under analise has the same language of the dataset/Europarl/en_GB.English-latn-EP7.utf8
The Number of bits needed to encode the text based on that model is 14800
```

### locatelang

```
Block initial posistion - final position 	 Language file
Time to count ocorrences: 3.20377
Time to calculate probabilities: 0.065803
Time to count ocorrences: 3.1231
Time to calculate probabilities: 0.055768
Time to count ocorrences: 3.2672
Time to calculate probabilities: 0.058261



0 - 10 has the same as the file: dataset/Europarl/pt_PT.Portugese-latn-EP7.utf8
10 - 20 has the same as the file: dataset/Europarl/pt_PT.Portugese-latn-EP7.utf8
20 - 30 has the same as the file: dataset/Europarl/pt_PT.Portugese-latn-EP7.utf8
30 - 40 has the same as the file: dataset/Europarl/pt_PT.Portugese-latn-EP7.utf8
40 - 50 has the same as the file: dataset/Europarl/es_ES.Spanish-latn-EP7.utf8
50 - 60 has the same as the file: dataset/Europarl/en_GB.English-latn-EP7.utf8
60 - 70 has the same as the file: dataset/Europarl/es_ES.Spanish-latn-EP7.utf8
70 - 80 has the same as the file: dataset/Europarl/pt_PT.Portugese-latn-EP7.utf8
80 - 90 has the same as the file: dataset/Europarl/pt_PT.Portugese-latn-EP7.utf8
90 - 100 has the same as the file: dataset/Europarl/en_GB.English-latn-EP7.utf8
100 - 110 has the same as the file: dataset/Europarl/en_GB.English-latn-EP7.utf8
110 - 120 has the same as the file: dataset/Europarl/en_GB.English-latn-EP7.utf8
120 - 130 has the same as the file: dataset/Europarl/en_GB.English-latn-EP7.utf8
130 - 140 has the same as the file: dataset/Europarl/es_ES.Spanish-latn-EP7.utf8
140 - 150 has the same as the file: dataset/Europarl/en_GB.English-latn-EP7.utf8
150 - 160 has the same as the file: dataset/Europarl/es_ES.Spanish-latn-EP7.utf8
160 - 170 has the same as the file: dataset/Europarl/en_GB.English-latn-EP7.utf8
170 - 180 has the same as the file: dataset/Europarl/en_GB.English-latn-EP7.utf8
180 - 190 has the same as the file: dataset/Europarl/es_ES.Spanish-latn-EP7.utf8
190 - 200 has the same as the file: dataset/Europarl/pt_PT.Portugese-latn-EP7.utf8
200 - 210 has the same as the file: dataset/Europarl/pt_PT.Portugese-latn-EP7.utf8
210 - 220 has the same as the file: dataset/Europarl/es_ES.Spanish-latn-EP7.utf8
220 - 230 has the same as the file: dataset/Europarl/en_GB.English-latn-EP7.utf8
230 - 240 has the same as the file: dataset/Europarl/es_ES.Spanish-latn-EP7.utf8
240 - 250 has the same as the file: dataset/Europarl/es_ES.Spanish-latn-EP7.utf8
250 - 260 has the same as the file: dataset/Europarl/pt_PT.Portugese-latn-EP7.utf8
260 - 270 has the same as the file: dataset/Europarl/en_GB.English-latn-EP7.utf8
270 - 280 has the same as the file: dataset/Europarl/en_GB.English-latn-EP7.utf8
280 - 290 has the same as the file: dataset/Europarl/es_ES.Spanish-latn-EP7.utf8
290 - 300 has the same as the file: dataset/Europarl/en_GB.English-latn-EP7.utf8
300 - 310 has the same as the file: dataset/Europarl/es_ES.Spanish-latn-EP7.utf8
310 - 320 has the same as the file: dataset/Europarl/pt_PT.Portugese-latn-EP7.utf8
320 - 330 has the same as the file: dataset/Europarl/es_ES.Spanish-latn-EP7.utf8
330 - 340 has the same as the file: dataset/Europarl/en_GB.English-latn-EP7.utf8
340 - 350 has the same as the file: dataset/Europarl/pt_PT.Portugese-latn-EP7.utf8
350 - 360 has the same as the file: dataset/Europarl/pt_PT.Portugese-latn-EP7.utf8
360 - 370 has the same as the file: dataset/Europarl/pt_PT.Portugese-latn-EP7.utf8
370 - 380 has the same as the file: dataset/Europarl/es_ES.Spanish-latn-EP7.utf8
```
