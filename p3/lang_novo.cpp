#include "fcm_novo.h"
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
		cerr << "Usage: ./build/p3/lang_novo <k> <alpha> <ToBeAnalizedTextFile> <LanguageTextFile>\n";
		return 1;
	}

	// Verify if k argumetn is a number
    if(isPositiveIntegerNumber(argv[1]) == false){
    	cerr << "The argumetn k must be a positive integer\nMinimal Usage: ./build/p3/lang_novo <k> <alpha> <ToBeAnalizedTextFile> <LanguageTextFile>\n";
        return 1;
    }

    // Verify if alpha is a number (integer or float)
    if(isNumber(argv[2]) == false){
    	cerr << "The argument alph must be a positive number\nMinimal Usage: ./build/p3/lang_novo <k> <alpha> <ToBeAnalizedTextFile> <LanguageTextFile>\n";
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
	Fcm language_model(k, alpha, LanguageTextFile, "prob_table");

	// Create the FCM for the analised text
	Fcm ToBeAnalizedTextFile_model(k, alpha, ToBeAnalizedTextFile);
	// Load the prob_table
	ToBeAnalizedTextFile_model.load_model("prob_table");
    clock_t start, end;
	// Calculate nBits
    start = clock();
	double nbits = ToBeAnalizedTextFile_model.calculate_nBits();
    end = clock();
    double time_taken = double(end - start) /double(CLOCKS_PER_SEC);
    cout << "Time to calculate nº bits: " << time_taken << setprecision(5) << endl;

	// Print the result
	cout << "Using the " << LanguageTextFile << " model, the under analise text would need " << nbits << " bits to be writen."<< endl;


};