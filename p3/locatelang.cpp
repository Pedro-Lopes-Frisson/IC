#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <unistd.h>
#include <vector>
#include <string>
#include "fcm_novo.h"
using namespace std;

static void print_help(){
	cerr << "Usage:" << endl;
	cerr << "\t ./build/p3/locatelang <ToBeAnalizedTextFile> -k <k>  -a <alpha> -b <blocksize> -l <LanguageTextFile> -r <Reference Texts>" << endl;
	cerr << "More that one flag can be used to specify more models to load or more reference texts to use" << endl;
	cerr << "But -l and -r cannot be used at the same time" << endl;

}




int main(int argc, char* argv[]){
	size_t order = 0, blockSize = 0;
	double alpha = 0;
	vector<string> modelsToLoad;
	vector<string> reference_texts;

	if( argc < 4 ){
		cerr << "You need to specify at least the executable and path of the text to classify, order of the model and a smoothing parameter" << endl;
		return -1 ;
	}
	string text = string(argv[1]);

	for(;;){
		switch(getopt(argc, argv, "k:a:b:r:l:h")) // note the colon (:) to indicate that 'b' has a parameter and is not a switch
			//
		{
			case 'k':
				order = atol(optarg);
				cout << order << endl;
				if(order < 0){
					cerr << "Value must be positive!" << endl;
					return -1;
				}
				continue;
			case 'a':
				alpha = atof(optarg);
				cout << alpha << endl;
				if(alpha < 0){
					cerr << "Value must be positive!" << endl;
					return -1;
				}
				continue;

			case 'b':
				blockSize = atol(optarg);
				cout << blockSize << endl;
				if(blockSize < 0){
					cerr << "Value must be positive!" << endl;
					return -1;
				}
				continue;
			case 'l':
				modelsToLoad.push_back(string(optarg));
				cout << optarg << endl;
				continue;
			case 'r':
				reference_texts.push_back(string(optarg));
				cout << optarg << endl;
				continue;
			case -1:
				break;

			case 'h':
			default:
				print_help();
				break;

		}
		break;
	}

	Fcm text_to_analyse(order,alpha, text.data());
	if(order == 0 || blockSize == 0 || alpha == (double)0){print_help(); return -1;} // do not allow any of these params to be 0
	if(modelsToLoad.size() == 0 && reference_texts.size() == 0){print_help(); return -1;} // do not allow -r and -l to be specified at the same time
	if(modelsToLoad.size() > 0 && reference_texts.size() > 0){print_help(); return -1;} // do not allow -r and -l to be specified at the same time
	//
	vector<vector<double>> bits_per_symbol_per_language;
	if(modelsToLoad.size() > 0){
		for(string f : modelsToLoad){
			text_to_analyse.load_model(f.data());
			bits_per_symbol_per_language.push_back(text_to_analyse.locate_lang_nBits(blockSize));
		}
	}
	vector<vector<double>> bits_per_symbol_per_language_processed;
	cout << "Block initial posistion - final position \t Language file" << endl;

	if(bits_per_symbol_per_language.size() > 0){
		for(size_t l = 0 ; l < bits_per_symbol_per_language.size(); l++){
			cout << endl;
			vector<double> vec = bits_per_symbol_per_language[l];
			for(size_t i = 0; i < vec.size(); i++){
				if(vec[i] == ceil( -(log2(alpha / (alpha * 27))))){
					vec[i] *= 0;
				}
			}
			bits_per_symbol_per_language[l] = vec;
		}

		vector<size_t> sums = vector<size_t>(bits_per_symbol_per_language.size());
		fill(sums.begin(), sums.end(), 0);
		size_t language_pos;

		for(size_t i = 0; i < bits_per_symbol_per_language[0].size(); i++){
			for(size_t l = 0 ; l < bits_per_symbol_per_language.size(); l++){
				sums[l] += bits_per_symbol_per_language[l][i];
			}
			if(i != 0 && i % blockSize == 0){
				int maxElementIndex = max_element(sums.begin(),sums.end()) - sums.begin();
				int maxElement = *max_element(sums.begin(), sums.end());

				cout << i-blockSize << " - " << i << " has the same as the file: " << modelsToLoad[maxElementIndex];
				cout << endl;
				fill(sums.begin(), sums.end(), 0);
			}
		}
		return 0;
	}


	if(reference_texts.size() > 0){
		string prob_table_file;
		for(string f : reference_texts){
			prob_table_file = f + "prob_table";
			Fcm references(order,alpha,f.data(), prob_table_file.data());
			
			text_to_analyse.load_model(prob_table_file.data());
			bits_per_symbol_per_language.push_back(text_to_analyse.locate_lang_nBits(blockSize));
		}
	}

	if(bits_per_symbol_per_language.size() > 0){
		for(size_t l = 0 ; l < bits_per_symbol_per_language.size(); l++){
			cout << endl;
			vector<double> vec = bits_per_symbol_per_language[l];
			for(size_t i = 0; i < vec.size(); i++){
				if(vec[i] == ceil( -(log2(alpha / (alpha * 27))))){
					vec[i] *= 0;
				}
			}
			bits_per_symbol_per_language[l] = vec;
		}

		vector<size_t> sums = vector<size_t>(bits_per_symbol_per_language.size());
		fill(sums.begin(), sums.end(), 0);
		size_t language_pos;

		for(size_t i = 0; i < bits_per_symbol_per_language[0].size(); i++){
			for(size_t l = 0 ; l < bits_per_symbol_per_language.size(); l++){
				sums[l] += bits_per_symbol_per_language[l][i];
			}
			if(i != 0 && i % blockSize == 0){
				language_pos = distance(sums.begin(), max_element(sums.begin(), sums.end()));
				cout << i-blockSize << " - " << i << " has the same as the file: " << reference_texts[language_pos];
				cout << endl;
				fill(sums.begin(), sums.end(), 0);
			}
		}
		return 0;
	}


	return -1;
}
