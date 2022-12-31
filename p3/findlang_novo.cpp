#include "fcm_novo.h"
#include <algorithm>
#include <unordered_map>
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


	cout << endl;
	

	// Create the FCM for the ToBeAnalizedTextFile
	Fcm ToBeAnalizedTextFile_model(k, alpha, ToBeAnalizedTextFile);
	// Vector to store number oof bits needed to write the ToBeAnalizedTextFile
	vector<size_t> nBits_needed;
	// Lets run throug all_LanguageTextFile
	for (int i = 0; i < n_Languages; i++){
		string model_save (all_LanguageTextFile[i] );
		model_save += "_model_prob_table";
		// Create the FCM for the language texts so we can obtain the probability table
		Fcm language_model(k, alpha, all_LanguageTextFile[i], model_save.data());
		// Load the prob table to the ToBeAnalizedTextFile
		ToBeAnalizedTextFile_model.load_model(model_save.data());
		// Calculate nBits needed
		double nbits = ToBeAnalizedTextFile_model.calculate_nBits();
		// Store the number of bits
		nBits_needed.push_back(nbits);
		// Print to check whats happening
		cout << "Using the " << all_LanguageTextFile[i]<< " model, the under analise text would need " << nbits << " bits to be writen."<< endl;

	}


	cout << endl;


	// Inicialize the variable to store the least number of bits
	double lowest_nBits = nBits_needed[0];
	// Store the index of the lowest number of bits
	int index = 0;
	// Lets see whats the language that was identified
	for (int i = 0; i < n_Languages; i++){

		if(nBits_needed[i] < lowest_nBits){
			// We store the lowest value in the variable
			lowest_nBits = nBits_needed[i];
			index = i;
		}

	}

	cout << endl;
	cout << "The language of the text under analise has the same language of the " << all_LanguageTextFile[index] << endl;
	cout << "The Number of bits needed to encode the text based on that model is " << lowest_nBits << endl;

};
