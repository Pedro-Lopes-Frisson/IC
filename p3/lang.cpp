#include "fcm.h"
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
	if(argc != 5){
		cerr << "Usage: ./build/p3/findlang <k> <alpha> <ToBeAnalizedTextFile> <LanguageTextFile>\n";
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
	// Store the Language Text
	char* LanguageTextFile= argv[4];
	// k argument
	int k = atoi(argv[1]);
	// alpha argument
	double alpha = atof(argv[2]);

	// Create the FCM for the language texts so we can obtain the probability table
	fcm f(k, alpha, LanguageTextFile, "file1.txt.out");
	// Count the ocorrencies of each context / letter
	f.count_occurrences();
	// Calculate the probabilities and store them in a map
	unordered_map <string, vector<double>> map_prob = f.calculate_probabilities();
	// Calculate the entropy of the language
	//double lang_entro = f.calculate_entropy();
	// Print the entropy of the language
	//cout << "Text Language: " << LanguageTextFile << ", Entropy: " << lang_entro << endl;

	//cout << endl;

	// Create the FCM for the under analisys text to get the entropy based on the probabilities of the languages texts
	fcm f_A(k, alpha, ToBeAnalizedTextFile, "file2.txt.out");
	// Count the ocorrencies of each context / letter on the under analises text
	f_A.count_occurrences();
	// Calculate entropy based on the probabilities of the language
	double analised_entro = f_A.calculate_entropy(map_prob);
    // Calculate entropy based on the probabilities of the language
    //double nbits = f_A.calculate_nBits(ToBeAnalizedTextFile ,map_prob);
    double nbits = f_A.calculate_nBits(map_prob);
	// Print the entropy of the under analisies text based on language probs
	cout << "Under Analisys Text number of bits to encode based on this language model " << LanguageTextFile << ": " << nbits << endl;

};