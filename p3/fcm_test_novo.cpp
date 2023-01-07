#include "fcm_novo.h"
#include <cstddef>
#include <cstdlib>
#include <string>
#include <iostream>

using namespace std;
void ctx_to_pos(string ctx, size_t *pos, int k);
int main() {
	//create a model
	//Fcm ingles (6,0.5, "dataset/Europarl/en_GB.English-latn-EP7.utf8", "English_model_6_0_001");
	//Fcm portugues (6,0.5, "dataset/Europarl/pt_PT.Portugese-latn-EP7.utf8", "Portuguese_model_6_0_001");
	//Fcm espanhol (6,0.5,    "dataset/Europarl/es_ES.Spanish-latn-EP7.utf8", "Spanish_model_6_0_001");
	//Fcm alemao (6,0.5,    "dataset/Europarl/de_DE.German-latn-EP7.utf8", "German_model_6_0_001");
	// this will by default count occurrences, calculate_probabilities and then save the mat_prob to Mopdel_example
	//
	
	
	Fcm floaded(6,0.5, "mini_example_ToBeAnalised_EN_with_PT.txt");
	floaded.load_model("English_model_6_0_001");
	// Now we have the model we created before loaded on this instance

	// calculate how many bits are needed to encode this file according to the loaded model
	floaded.calculate_nBits();
	ofstream file ("values_bits_en_pt_es");

	vector<double> ents_EN;
	vector<double> nBits_s_EN;
	nBits_s_EN = floaded.locate_lang_nBits(8);
	//ents_EN = floaded.locate_lang_ent();

	cout << "Ents_EN";
	cout << endl;
	size_t cum_sum = 0;
	double average_bs = 0;
	for (size_t i = 0; i < nBits_s_EN.size(); i++ ) {
		if (i % 8 == 0 and i != 0){
			average_bs = ((double) cum_sum) / 8;
			cum_sum = 0;
			cout << i-8 << " - " << i << " Avg num bits: " << average_bs;
			cout << endl;
		}
		cum_sum += nBits_s_EN[i];
	}



	// Load another model and compute number of bits
	floaded.load_model("Portuguese_model_6_0_001");
	floaded.calculate_nBits();
	vector<double> ents_PT;
	vector<double> nBits_s_PT;
	nBits_s_PT = floaded.locate_lang_nBits(8);
	cout << endl;
	cout << "PT MODEL" << endl;
	for (size_t i = 0; i < nBits_s_PT.size(); i++ ) {
		if (i % 8 == 0 and i != 0){
			average_bs = ((double) cum_sum) / 8;
			cum_sum = 0;
			cout << i-8 << " - " << i << " Avg num bits: " << average_bs;
			cout << endl;
		}
		cum_sum += nBits_s_PT[i];
	}


	// Load another model and compute number of bits
	floaded.load_model("Spanish_model_6_0_001");
	floaded.calculate_nBits();
	vector<double> ents_ES;
	vector<double> nBits_s_ES;
	nBits_s_ES = floaded.locate_lang_nBits(8);
	cout << endl;
	cout << "German MODEL" << endl;
	for (size_t i = 0; i < nBits_s_ES.size(); i++ ) {
		if (i % 8 == 0 and i != 0){
			average_bs = ((double) cum_sum) / 8;
			cum_sum = 0;
			cout << i-8 << " - " << i << " Avg num bits: " << average_bs;
			cout << endl;
		}
		cum_sum += nBits_s_ES[i];
		file << nBits_s_EN[i] << " " << nBits_s_PT[i] << " " << nBits_s_ES[i] << endl;
	}


};



void ctx_to_pos(string ctx, size_t *pos, int k){
	// a -> 0
	// b -> 1
	// aa -> 0 + 1 -> 0 + 0 0 + 1
	// aaa -> 0 + 1 + -> 0 + 0 0 + 1
	size_t space_pos = 27 - 1;
	size_t pos_t = 0;
	for (size_t i = 0; i < k; i++) {
		if (ctx[i] == ' ') {
			pos_t += (27-1) * pow(27, i);
		}
		else{
			pos_t += (ctx[i] - (size_t)'a') * pow(27, i);
		}
	}
	*pos = pos_t;
}
