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
		             vector < size_t >> (ctx, vector<size_t>(ALPHABET_LENGTH, 0)));

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
		if (i == ALPHABET_LENGTH - 1)
			newPrefix = prefix + ' ';
		else
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
	char ctx[k + 1];
	int i = 0;
	for (auto c: context) {
		ctx[i] = c;
		i++;
	}
	ctx[k] = '\0';
	string string_ctx(ctx);
	// check if context exists
	if (table.find(string_ctx) == table.end()) {
		// context does not exist create it
		// create vector
		vector <size_t> new_vector(ALPHABET_LENGTH);

		fill(new_vector.begin(), new_vector.end(), 0);
		table.insert(std::pair < std::string,
		             std::vector < size_t >> (string_ctx, new_vector));

	}

	// increment counter at this point is safe just to increment it
	if (*new_char != ' ')
		table[string_ctx][(size_t)(*new_char) - ALPHABET_START]++;
	else
		table[string_ctx][ALPHABET_LENGTH - 1]++; // space
	return;
}

unordered_map <string, vector<double>> fcm::calculate_probabilities() {
	size_t cumulative_sum;
	for (auto &entry: table) {
		//first is the context
		//second is the vector

		// begin of the vector, end of the vector, sum starts at smoothing_parameter instead of 0

		cumulative_sum = accumulate(entry.second.begin(), entry.second.end(),
		                            0);

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

		double pb, paeb;
		// for each value inside vector calculate probability
		size_t cnt;
		for (size_t i = 0; i < entry.second.size(); i++) {
			cnt = entry.second[i];
			pb = cumulative_sum / chars_read;
			paeb = (cnt + smoothing_parameter) /
			       (cumulative_sum + (ALPHABET_LENGTH * smoothing_parameter));

			table_probabilities[entry.first][i] = (cnt + smoothing_parameter)
			                                      / (cumulative_sum +
			                                         (ALPHABET_LENGTH * smoothing_parameter));
		}

	}

	return table_probabilities;
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

double fcm::calculate_entropy(unordered_map <string, vector<double>> map) {
	// TODO: FIX THIS
	model_entropy = 0;
	double ctx_entropy = 0;
	double prob;
	double cum_sum = 0;

	for (auto &entry: map) {
		ctx_entropy = 0;
		for (size_t i = 0; i < entry.second.size(); i++) {
			// if count was zero then don't use it to calculate the entropy
			if (table.find(entry.first)->second[i] == 0) continue;
			prob = entry.second[i];

			ctx_entropy += -(prob * log2(prob));
		}
		cum_sum = accumulate(table[entry.first].begin(), table[entry.first].end(), 0);
		model_entropy += (cum_sum / chars_read) * ctx_entropy;
	}
	//cout << "Entropy: " << model_entropy << endl;

	return model_entropy;
}

double fcm::calculate_entropy() {
	// TODO: FIX THIS
	model_entropy = 0;
	double ctx_entropy = 0;
	double prob;
	double cum_sum = 0;

	for (auto &entry: table_probabilities) {
		ctx_entropy = 0;
		for (size_t i = 0; i < entry.second.size(); i++) {
			// if count was zero then don't use it to calculate the entropy
			if (table.find(entry.first)->second[i] == 0) continue;
			prob = entry.second[i];

			ctx_entropy += -(prob * log2(prob));
		}
		cum_sum = accumulate(table[entry.first].begin(), table[entry.first].end(), 0);
		model_entropy += (cum_sum / chars_read) * ctx_entropy;
	}
	//cout << "Entropy: " << model_entropy << endl;

	return model_entropy;
}

void fcm::count_occurrences() {
	int i = 0;
	char c;
	// read first char
	c = tolower(file_in.get());
	//cout << "First: |" << c << "|" << endl;
	chars_read++;

	// fill buffer
	while (i < this->k && c != EOF) {
		if (isalpha(c) || c == ' ') {
			add_to_context(&c);
			//cout << "Valid: |" << c << "|" << endl;
			c = tolower(file_in.get());
			chars_read++;
			i++;
		} else {
			//cout << "2 Not Valid: |" << c << "|" << endl;
			c = tolower(file_in.get());
		}
	}

	// Count and add to context
	while (c != EOF) {
		if (!isalpha(c) && c != ' ') {
			//cout << "3 Not Valid: |" << c << "|" << endl;
			c = tolower(file_in.get());
			continue;
		}
		chars_read++;
		increment_counter(&c);
		add_to_context(&c);
		c = tolower(file_in.get());
		//cout << "|" << c << "|" << endl;
	}

	// discount EOF
	chars_read--;
}


void fcm::print_probabilities() {
	cout << "TABLE OF PROBABILITIES" << endl;
	cout << "| Context/Symbol | ";
	for (size_t i = 0; i < ALPHABET_LENGTH; i++) {
		if (i == ALPHABET_LENGTH - 1)
			cout << "<SPACE>|";
		else
			cout << (char) (ALPHABET_START + i) << "  |";
	}
	cout << endl;

	for (auto &entry: table_probabilities) {
		cout << "| " << entry.first << "           |";
		for (auto prob: entry.second)
			cout << " " << prob << " |";
		cout << endl;
	}
	cout << endl;
}

void fcm::print_occurrences() {
	cout << "TABLE OF OCCURRENCES" << endl;
	cout << "| Context/Symbol |    ";
	for (size_t i = 0; i < ALPHABET_LENGTH; i++) {
		if (i == ALPHABET_LENGTH - 1)
			cout << "<SPACE>  |";
		else
			cout << (char) (ALPHABET_START + i) << "  |";
	}
	cout << endl;

	for (auto &entry: table) {
		cout << " | " << entry.first << " |";
		for (auto prob: entry.second)
			cout << " " << prob << " |";
		cout << endl;
	}
	cout << endl;
}

//int main() {
//	fcm f(5, 0.0001, "example.txt", "file1.txt.out");
//	f.count_occurrences();
//	f.print_occurrences();
//	f.calculate_probabilities();
//	f.print_probabilities();
//	f.calculate_entropy();
//};
