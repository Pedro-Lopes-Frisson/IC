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
		cerr << "Minimal Usage: ./build/p3/locatelang <k> <alpha> <ToBeAnalizedTextFile> <LanguageTextFile>\n";
		return 1;
	}

	// Verify if k argumetn is a number
    if(isPositiveIntegerNumber(argv[1]) == false){
    	cerr << "The argumetn k must be a positive integer\nMinimal Usage: ./build/p3/locatelang <k> <alpha> <ToBeAnalizedTextFile> <LanguageTextFile>\n";
        return 1;
    }

    // Verify if alpha is a number (integer or float)
    if(isNumber(argv[2]) == false){
    	cerr << "The argument alph must be a positive number\nMinimal Usage: ./build/p3/locatelang <k> <alpha> <ToBeAnalizedTextFile> <LanguageTextFile>\n";
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
	// Define a vectro to store all entropies from languages
	vector<double> language_entropies;

	// Store all LanguageTextFile
	for (int i = 0; i < n_Languages; i++){

		//Push the file into the vector
		all_LanguageTextFile.push_back(argv[i+4]);
	}

	// Vector to store the probabilities maps
	vector<unordered_map <string, vector<double>>> prob_maps;

	// Lets run throug all_LanguageTextFile and calculate the entropy
	for (int i = 0; i < n_Languages; i++){

		// Create the FCM for the language texts so we can obtain the probability table
		fcm f(k, alpha, all_LanguageTextFile[i], "file1.txt.out");
		// Count the ocorrencies of each context / letter
		f.count_occurrences();
		// Calculate the probabilities and store them in a map
		unordered_map <string, vector<double>> map_prob = f.calculate_probabilities();
		prob_maps.push_back(map_prob);
		// Calculate the entropy of the language
		double lang_entro = f.calculate_entropy();
		language_entropies.push_back(lang_entro);

		cout << "Entropy of language model " << all_LanguageTextFile[i] << ": " << lang_entro << endl;

	}

	cout << endl;

	// Create the FCM for the under analisys text to get the entropy based on the probabilities of the languages texts
	fcm f_A(k, alpha, ToBeAnalizedTextFile, "file2.txt.out");
	// Count the ocorrencies of each context / letter on the under analises text
	f_A.count_occurrences();
	// Map to store all instantaneous entropys for each language
	unordered_map <int, vector<double>> inst_entropy_maps;

	// Get the instant entropy of the under analises text for each language model
	for (int i = 0; i < n_Languages; i++){

		vector<double> inst_entropy = f_A.calculate_inst_entropy(prob_maps[i]);
		inst_entropy_maps.insert({ i, inst_entropy});

	}

	// Print map to see if everything went good
	for (auto itr = inst_entropy_maps.begin(); itr != inst_entropy_maps.end(); ++itr) {
        cout << itr->first << '\t';
        for (long unsigned int i = 0; i < itr->second.size(); i++){
        	cout << itr->second[i]<< ", ";
        }
        cout << endl;
    }




};