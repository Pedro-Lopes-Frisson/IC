//
// Created by whoknows on 12/5/22.
//

#include "fcm.h"
// cumulative sum
#include <numeric>
#include <iostream>
#include <unistd.h>

using namespace std;

fcm::fcm(int order, double smooth_parameter, const char *fIn
         , const char *fOut) {
	this->k = order;
	this->smoothing_parameter = smooth_parameter;
	context.resize(k);
	open_file(fIn, 0);
	open_file(fOut, 1);
	vector <string> contexts;
	fcm::possible_contexts(contexts, order, "");
	for (auto ctx: contexts) {
		table.insert(pair < string,
		             vector < size_t >> (ctx, vector<size_t>(ALPHABET_LENGTH, smoothing_parameter)));

		table_probabilities.insert(pair < string,
		                           vector < double >> (ctx, vector<double>(ALPHABET_LENGTH, 0.0f)));
	}


}

// The main recursive method
// to print all possible
// strings of length k
void
fcm::possible_contexts(vector <string> &contexts, int order, string prefix) {

	// Base case: k is 0,
	// print prefix
	if (order == 0) {
		contexts.push_back(prefix);
		return;
	}

	// One by one add all characters
	// from set and recursively
	// call for k equals to k-1
	for (size_t i = 0; i < ALPHABET_LENGTH; i++) {
		string newPrefix;

		// Next character of input added
		newPrefix = prefix + (char) (ALPHABET_START + i);

		// k is decreased, because
		// we have added a new character
		fcm::possible_contexts(contexts, order - 1, newPrefix);
	}

}


void fcm::open_file(const char *filename, int mode) {
	if (mode == 0) {
		//open file in input mode (read from)
		file_in.open(filename, ios_base::in);
		if (!file_in.is_open()) {
			cerr << "File open operation failed; " << filename << endl;
		}
		return;
	}
	// open file in append mode
	file_out.open(filename, ios_base::app);
}

void fcm::add_to_context(char *new_char) {
	context.erase(context.begin());
	context.push_back(*new_char);
}

void fcm::increment_counter(const char *new_char) {
	std::cout << "Context: " << context.data() << "NEW CHAR: " << *new_char << endl;
	// check if context exists
	if (table.find(context.data()) == table.end()) {
		// context does not exist create it
		// create vector
		vector <size_t> new_vector(ALPHABET_LENGTH);

		// fill vector with default_value smoothing parameter
		// so that there are no probabilities which are zero
		fill(new_vector.begin(), new_vector.end(), smoothing_parameter);
		table.insert(std::pair < std::string,
		             std::vector < size_t >> (context.data(), new_vector));

	}

	// increment counter at this point is safe just to increment it
	table[context.data()][(size_t)(*new_char) - ALPHABET_START]++;
	return;
}

void fcm::calculate_probabilities() {
	size_t cumulative_sum;
	for (auto &entry: table) {
		//first is the context
		//second is the vector

		// begin of the vector, end of the vector, sum starts at smoothing_parameter instead of 0

		cumulative_sum = accumulate(entry.second.begin(), entry.second.end(),
		                            smoothing_parameter);

		// key did not exist create entry
		if (table_probabilities.find(entry.first) == table_probabilities.end()) {
			// context does not exist create it
			// create vector
			vector<double> new_vector(ALPHABET_LENGTH);
			// fill vector
			fill(new_vector.begin(), new_vector.end(), (double) 0);

			table_probabilities.insert(std::pair < std::string, std::vector < double
					>> (entry.first, new_vector));
		}

		// for each value inside vector calculate probability
		size_t cnt;
		for (size_t i = 0; i < entry.second.size(); i++) {
			// TODO: FIX FORMULA
			cnt = entry.second[i];
			table_probabilities[entry.first][i] =
					(cnt + smoothing_parameter) /
					(cumulative_sum + (ALPHABET_LENGTH * smoothing_parameter));
		}

	}
}

//oid fcm::print_probabilities() {
// //print header
// cout << "\t";
// for (size_t i = 0; i < ALPHABET_LENGTH; i++) {
//   cout << (char) (i + ALPHABET_START) << " |";
// }
// cout << endl;
// for (auto &entry: table) {
//   cout << entry.first << " | ";
//   for (auto cnt: entry.second) {
//     cout << cnt << " | ";
//   }
// }
// cout << endl;
//

void fcm::calculate_entropy() {
	model_entropy = 0;
	for (auto &entry: table_probabilities) {
		for (auto prob: entry.second) {
			model_entropy += -(prob * log2(prob));
		}
	}
	cout << model_entropy << endl;
}

void fcm::count_occurrences() {
	int i = 0;
	char c;
	// read first char
	c = file_in.get();
	// fill buffer
	while (i < this->k && c != EOF) {
		add_to_context(&c);
		c = tolower(file_in.get());
		i++;
	}

	// Count and add to context
	while (isalpha(c)) {
		increment_counter(&c);
		add_to_context(&c);
		c = tolower(file_in.get());

	}

}


void fcm::print_probabilities() {
	cout << "TABLE OF PROBABILITIES" << endl;
	cout << "Context/Symbol | ";
	for (size_t i = 0; i < ALPHABET_LENGTH; i++) {
		cout << (char) (ALPHABET_START + i) << "  |";
	}
	cout << endl;

	for (auto &entry: table_probabilities) {
		cout << entry.first << "           |";
		for (auto prob: entry.second)
			cout << " " << prob << " |";
	}
	cout << endl;
}

void fcm::print_occurrences() {
	cout << "TABLE OF OCCURRENCES" << endl;
	cout << "Context/Symbol |    ";
	for (size_t i = 0; i < ALPHABET_LENGTH; i++) {
		cout << (char) (ALPHABET_START + i) << " | ";
	}
	cout << endl;

	for (auto &entry: table) {
		cout << entry.first << "                 |";
		for (auto prob: entry.second)
			cout << " " << prob << " |";
		cout << endl;
	}
	cout << endl;
}

int main() {
	fcm f(1, 1, "file1.txt", "file1.txt.out");
	f.count_occurrences();
	f.print_occurrences();
	f.calculate_probabilities();
	//f.print_probabilities();
};
