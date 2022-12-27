#include "fcm_novo.h"
#include <cstddef>
#include <string>
#include <iostream>

using namespace std;
int main() {
	//create a model
	Fcm f (3,0.001, "example.txt", "Model_example");
	Fcm portugues (3,0.001, "mini_example_PT.txt", "PT_model");
	// this will by default count occurrences, calculate_probabilities and then save the mat_prob to Mopdel_example
	
	Fcm floaded(3,0.001, "txt");
	floaded.load_model("Model_example");
	// Now we have the model we created before loaded on this instance

	// calculate how many bits are needed to encode this file according to the loaded model
	floaded.calculate_nBits();

	vector<double> ents_EN;
	vector<double> nBits_s_EN;
	nBits_s_EN = floaded.locate_lang_nBits();
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
	nBits_s_PT = floaded.locate_lang_nBits();
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
