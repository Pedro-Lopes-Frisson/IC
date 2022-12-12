#include "fcm.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){

	// Check if number of arguments is minimal 3, the path, the text to be analised
	// and the text that represents the language
	if(argc < 3){
		cerr << "Minimal Usage: /build/p3/findlang <ToBeAnalizedTextFile> <LanguageTextFile>\n";
		return 1;
	}

	// Store the ToBeAnalizedTextFile
	string ToBeAnalizedTextFile = argv[1];

	// Read the number of arguments
	int n_Languages = argc - 2; // -2 because the path and the outfile also count

	// Store the text files on a vector
	vector<char *> all_LanguageTextFile;

	// Store all LanguageTextFile
	for (int i = 0; i < n_Languages; i++){

		//Push the file into the vector
		all_LanguageTextFile.push_back(argv[i+2]);
	}

	// Vector to store the entropies of each file
	vector<double> all_LanguageTextFile_entropies;

	// Lets run throug all_LanguageTextFile and calculate the entropy
	for (int i = 0; i < all_LanguageTextFile.size(); i++){

		// Create the FCM
		fcm f(3, 1, all_LanguageTextFile[i], "file1.txt.out");
		f.count_occurrences();
		f.print_occurrences();
		f.calculate_probabilities();
		f.print_probabilities();
		all_LanguageTextFile_entropies.push_back(f.calculate_entropy());

	}
}