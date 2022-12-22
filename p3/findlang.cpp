#include "fcm.h"
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <string>

using namespace std;

// Function to verify if a string is a positive integer number
bool isPositiveIntegerNumber(string number) {
    int i = 0;

    //checking for negative numbers
    if (number[0] == '-')
        return false;
    for (; number[i] != 0; i++){

        if (!isdigit(number[i]))
            return false;
    }
    return true;
}

// Function to verify if a string is a positive number (integer or double)
bool isNumber(string number) {

    //checking for negative numbers
    if (number[0] == '-'){
    	return false;
    }

    //Count the nuber of . or ,
    int point_count = 0;
    for (long unsigned int i = 0; i < number.size(); i++){

    	if(number[i] == '.' || number[i] == ','){
    		point_count++;
    	}
    	else if(!isdigit(number[i])){
    		return false;
    	}

    	// Verify if there was more than one point
    	if(point_count > 1){
    		return false;
    	}
    }
    return true;
}



int main(int argc, char *argv[]){

	// Check if number of arguments is minimal 3, the path, the text to be analised
	// and the text that represents the language
	if(argc < 4){
		cerr << "Minimal Usage: ./build/p3/findlang <k> <alpha> <ToBeAnalizedTextFile> <LanguageTextFile>\n";
		return 1;
	}

	// Verify if k argumetn is a number
    if(isPositiveIntegerNumber(argv[1]) == false){
    	cerr << "The argumetn k must be a positive integer\nMinimal Usage: ./build/p3/findlang <k> <alpha> <ToBeAnalizedTextFile> <LanguageTextFile>\n";
        return 1;
    }

    // Verify if alpha is a number (integer or float)
    if(isNumber(argv[2]) == false){
    	cerr << "The argument alph must be a positive number\nMinimal Usage: ./build/p3/findlang <k> <alpha> <ToBeAnalizedTextFile> <LanguageTextFile>\n";
        return 1;
    }

	// Store the ToBeAnalizedTextFile
	char* ToBeAnalizedTextFile = argv[3];
	// k argument
	int k = atoi(argv[1]);
	// alpha argument
	double alpha = atof(argv[2]);

	// Read the number of arguments
	int n_Languages = argc - 4; // -4 because the path, k, alpha and ToBeAnalizedTextFile
	// Store the text files on a vector
	vector<char*> all_LanguageTextFile;

	// Store all LanguageTextFile
	for (int i = 0; i < n_Languages; i++){

		//Push the file into the vector
		all_LanguageTextFile.push_back(argv[i+4]);
	}

	// Define a vectro to store all entropies from languages
	vector<double> language_entropies;
	// Define a vectro to store all entropies from textd analized
	vector<size_t> analised_nBits_needed;
	// Vector to store the probabilities maps
	vector<unordered_map <string, vector<double>>> prob_maps;
	double _bits = 0;
	// Lets run throug all_LanguageTextFile and calculate the entropy
	for (int i = 0; i < n_Languages; i++){

		// Create the FCM for the language texts so we can obtain the probability table
		fcm f(k, alpha, all_LanguageTextFile[i], "file1.txt.out");
		// Count the ocorrencies of each context / letter
		f.count_occurrences();
		// Calculate the probabilities and store them in a map
		unordered_map <string, vector<double>> map_prob = f.calculate_probabilities();
		// Calculate Number of bits
		_bits = f.calculate_nBits(ToBeAnalizedTextFile);
		// Store the number of bits
		//analised_nBits_needed.push_back(_bits);
		// Store the prob map
		prob_maps.push_back(map_prob);
		// Calculate the entropy of the language
		double lang_entro = f.calculate_entropy();
		// Store the language entropy in a vector to future comparison
		language_entropies.push_back(lang_entro);
		// Print Info
		//cout << "Number of bits to write the ToBeAnalizedTextFile using the "  << all_LanguageTextFile[i] << ": " << _bits << endl;

	}

	cout << endl;

	// Create the FCM for the under analisys text to get the entropy based on the probabilities of the languages texts
	fcm f_A(k, alpha, ToBeAnalizedTextFile, "file2.txt.out");
	// Count the ocorrencies of each context / letter on the under analises text
	f_A.count_occurrences();
	//
	for (int i = 0; i < n_Languages; i++){
	
		// Calculate entropy based on the probabilities of the language
		double nbits = f_A.calculate_nBits(ToBeAnalizedTextFile ,prob_maps[i]);
		// Store the under analies text entropy in a vector to future comparison
		analised_nBits_needed.push_back(nbits);
		// Print the entropy of the under analisies text based on language probs
		cout << "Bits needed " << nbits << ", with Language model " << all_LanguageTextFile[i] << endl;
	
	}
	// Chose the language wich has the higher entropy
	// Variable to store the highest entropy
	//
	double lowest_nBits = analised_nBits_needed[0];
	int index = 0;
	for (int i = 0; i < n_Languages; i++){

		if(analised_nBits_needed[i] < lowest_nBits){
			// We store the highest value in the variable
			lowest_nBits = analised_nBits_needed[i];
			index = i;
		}

	}
	cout << endl;
	cout << "The language of the text under analise is the same as the language in " << all_LanguageTextFile[index] << endl;
	cout << "The Number of bits needed to encode the text " << all_LanguageTextFile[index] << " was " << lowest_nBits << endl;

};
