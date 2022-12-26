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

	// Load another model and compute number of bits
	floaded.load_model("PT_model");
	floaded.calculate_nBits();

};
