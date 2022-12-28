#include "fcm_novo.h"
#include <cstddef>
#include <cstdlib>
#include <string>
#include <iostream>

using namespace std;
void ctx_to_pos(string ctx, size_t *pos, int k);
int main() {
	//create a model
	Fcm f (6,0.001, "mini_example_GER.txt", "Model_example");
	Fcm portugues (6,0.001, "mini_example_PT.txt", "PT_model");
	// this will by default count occurrences, calculate_probabilities and then save the mat_prob to Mopdel_example
	//
	
	
	Fcm floaded(6,0.001, "mini_example_ToBeAnalised_GER_with_PT.txt");
	floaded.load_model("Model_example");
	// Now we have the model we created before loaded on this instance

	// calculate how many bits are needed to encode this file according to the loaded model
	floaded.calculate_nBits();

	size_t pos;
	ctx_to_pos("aaa", &pos, 3 );
	cout << "aaa" << pos << endl;
	ctx_to_pos("a", &pos, 1 );
	cout << "a" << pos << endl;

	vector<double> ents_EN;
	vector<double> nBits_s_EN;
	nBits_s_EN = floaded.locate_lang_nBits(3);
	ents_EN = floaded.locate_lang_ent();

	cout << "Ents_EN";
	cout << endl;
	for (auto _ents :ents_EN) {
		cout << _ents << endl;		
	}

	cout << "NBits_EN";
	cout << endl;
	for (auto _bits : nBits_s_EN) {
		cout << _bits << endl;		
	}



	// Load another model and compute number of bits
	floaded.load_model("PT_model");
	floaded.calculate_nBits();
	vector<double> ents_PT;
	vector<double> nBits_s_PT;
	nBits_s_PT = floaded.locate_lang_nBits(3);
	ents_PT = floaded.locate_lang_ent();

	cout << "Ents_PT";
	cout << endl;
	for (auto _ents :ents_PT) {
		cout << _ents << endl;		
	}

	cout << "NBits_PT";
	cout << endl;
	for (auto _bits : nBits_s_PT) {
		cout << _bits << endl;		
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
