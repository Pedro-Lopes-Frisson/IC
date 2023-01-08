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
