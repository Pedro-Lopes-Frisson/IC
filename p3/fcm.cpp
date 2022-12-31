//
// Created by whoknows on 12/5/22.
//

#include "fcm.h"
// cumulative sum
#include <boost/archive/text_iarchive.hpp>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <ios>
#include <math.h>
#include <numeric>
#include <iostream>
#include <unistd.h>
#include <utility>

using namespace std;

fcm::fcm(int order, double smooth_parameter, const char *fIn
         , const char *fOut) {
	this->k = order;
	this->smoothing_parameter = smooth_parameter;
	context.resize(k);
	open_file(fIn, 0);
	if(fOut != NULL)
		open_file(fOut, 1);

}

void fcm::ctx_to_pos(string ctx, size_t *pos){
	// a -> 0
	// b -> 1
	// aa -> 0 + 1 -> 0 + 0 0 + 1
	size_t space_pos = ALPHABET_LENGTH - 1;
	size_t pos_t = 0;
	for (size_t i = 0; i < k; i++) {
		if (ctx[i] == ' ') {
			pos_t += (ALPHABET_LENGTH-1) * pow(ALPHABET_LENGTH, i);
		}
		else{
			pos_t += (ctx[i] - ALPHABET_START) * pow(ALPHABET_LENGTH, i);
		}
	}
	*pos = pos_t;
}

void :: fcm::pos_to_ctx(size_t pos, std::string &ctx){
	size_t ch;
	for (int i = 0; i < k; i++) {
		ch = ((unsigned int) (pos / pow(ALPHABET_LENGTH, i)) % ALPHABET_LENGTH);
		ctx += (ch != 26) ? ch + ALPHABET_START : ' ';
		pos -= ch * pow(ALPHABET_LENGTH, i);
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
	size_t key;
	ctx_to_pos(ctx,&key);
	if (table.find(key) == table.end()) {
		// context does not exist create it
		// create vector
		vector <size_t> new_vector(ALPHABET_LENGTH);

		fill(new_vector.begin(), new_vector.end(), 0);
		table.insert(pair < size_t,
		             vector < size_t >> (key, new_vector));

	}

	// increment counter at this point is safe just to increment it
	if (*new_char != ' ')
		table[key][(size_t)(*new_char) - ALPHABET_START]++;
	else
		table[key][ALPHABET_LENGTH - 1]++; // space
	return;
}

map <size_t, vector<double>> fcm::calculate_probabilities() {
	size_t cumulative_sum, key;
	ctx_to_pos(context.data(), &key);
	for (size_t i = 0; i < pow(ALPHABET_LENGTH, k) ; i ++) {
		//first is the context
		//second is the vector


		if (table.find(i) == table.end()) {
			//table.insert(pair<size_t, vector<size_t>>(i, vector<size_t>(ALPHABET_LENGTH, 0)));
			//table_probabilities.insert(pair<size_t, vector<double>>(i, vector<double>(ALPHABET_LENGTH, smoothing_parameter / (ALPHABET_LENGTH * smoothing_parameter))));
			continue;
		}
		// begin of the vector, end of the vector, sum starts at smoothing_parameter instead of 0
		auto &entry = *table.find(i);

		cumulative_sum = accumulate(entry.second.begin(), entry.second.end(),
		                            0);

		if (table_probabilities.find(i) == table_probabilities.end()) {
			table_probabilities.insert(pair<size_t, vector<double>>(i, vector<double>(ALPHABET_LENGTH, smoothing_parameter / (ALPHABET_LENGTH * smoothing_parameter))));
		}
		double pb, paeb;
		// for each value inside vector calculate probability
		size_t cnt;
		for (size_t j = 0; j < entry.second.size(); j++) {
			cnt = entry.second[j];
			pb = cumulative_sum / (double) chars_read;
			paeb = (cnt + smoothing_parameter) /
			       (cumulative_sum + (ALPHABET_LENGTH * smoothing_parameter));

			table_probabilities[entry.first][j] = (cnt + smoothing_parameter)
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

double fcm::calculate_entropy
(map <size_t, vector<double>> map) {
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
	cout << "Entropy: " << model_entropy << endl;

	return model_entropy;
}

double fcm::calculate_entropy() {
	// TODO: FIX THIS
	model_entropy = 0;
	double ctx_entropy = 0;
	double prob;
	double cum_sum = 0;
	vector<double> vc_default(ALPHABET_LENGTH, smoothing_parameter);
	vector<double> vp_default(ALPHABET_LENGTH, smoothing_parameter / (smoothing_parameter * ALPHABET_LENGTH));

	for (auto &entry: table_probabilities) {
		ctx_entropy = 0;
		for (size_t i = 0; i < entry.second.size(); i++) {
			// if count was zero then don't use it to calculate the entropy
			//if (table.find(entry.first) == table.end()) continue;
			prob = entry.second[i];

			ctx_entropy += -(prob * log2(prob));
		}
		cum_sum = accumulate(table[entry.first].begin(), table[entry.first].end(), 0);
		model_entropy += (cum_sum / chars_read) * ctx_entropy;
	}
	cout << "Entropy: " << model_entropy << endl;

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
	string s;
	for (size_t i = 0; i < pow(ALPHABET_LENGTH, k); i++) {
		if (table_probabilities.count(i) == 0){
			// entry does not exist
			vector<double> new_vector(ALPHABET_LENGTH);
			// fill vector
			fill(new_vector.begin(), new_vector.end(), (double) 0);

			table_probabilities.insert(std::pair < size_t, vector < double>> (i , new_vector));
		}
		auto &entry = *table_probabilities.find(i);
		pos_to_ctx(entry.first, s);
		cout << "| " << s << "           |";
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
	string s;
	for (size_t i = 0; i < pow(ALPHABET_LENGTH, k); i++) {
		if (table.count(i) == 0){
			// entry does not exist
			table.insert(pair<size_t, vector<size_t>> (i, vector<size_t>(ALPHABET_LENGTH, 0)));
		}
		auto &entry = *table.find(i);
		pos_to_ctx(entry.first, s);
		cout << " | " << s << " |";
		for (auto prob: entry.second)
			cout << " " << prob << " |";
		cout << endl;
		s.clear();
	}
	cout << endl;
}

double fcm::get_prob(char next_char){
	char ctx[k + 1];
	int i = 0;
	for (auto c: context) {
		ctx[i] = c;
		i++;
	}
	ctx[k] = '\0';
	string string_ctx(ctx);
	size_t k;
	ctx_to_pos(context.data(), &k);
	for(size_t i = 0; i < ALPHABET_LENGTH; i++)
	// check if context exists
	if(next_char == ' '){
		return table_probabilities[k][ALPHABET_LENGTH - 1];
	}

return table_probabilities[k][(size_t)next_char - ALPHABET_START];
}

double fcm::get_prob(char next_char,unordered_map <string, vector<double>> map ){
	char ctx[k + 1];
	int i = 0;
	for (auto c: context) {
		ctx[i] = c;
		i++;
	}
	ctx[k] = '\0';
	string string_ctx(ctx);
	// check if context exists
	if(next_char == ' ')
		return map[ctx][ALPHABET_LENGTH - 1];
	return map[ctx][ (size_t)next_char - ALPHABET_START];
}

double fcm::calculate_nBits(char * fToClassify,unordered_map <string, vector<double>> map  ){
	// GoTo begin of file
	file_in.clear();
	file_in.seekg(0);

	int i = 0;
	char c;
	// read first char
	c = tolower(file_in.get());
	//cout << "First: |" << c << "|" << endl;
	chars_read++;
	size_t num_Bits = 0;


	// fill buffer (context)
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

	double prob;
	// Count and add to context
	while (c != EOF) {
		if (!isalpha(c) && c != ' ') {
			//cout << "3 Not Valid: |" << c << "|" << endl;
			c = tolower(file_in.get());
			continue;
		}
		chars_read++;
		prob = get_prob(c, map);

		num_Bits += ceil(-log2(get_prob(c, map)));
		add_to_context(&c);
		c = tolower(file_in.get());
		//cout << "|" << c << "|" << endl;
	}
	cout << "Num Bits: " << num_Bits << endl;
	// discount EOF
	chars_read--;
	return num_Bits;
}

double fcm::calculate_nBits(char * fToClassify){
	ifstream fIn {fToClassify, ios_base::in};
	int i = 0;
	char c;
	// read first char
	c = tolower(fIn.get());
	//cout << "First: |" << c << "|" << endl;
	chars_read++;
	size_t num_Bits = 0;


	// fill buffer (context)
	while (i < this->k && c != EOF) {
		if (isalpha(c) || c == ' ') {
			add_to_context(&c);
			//cout << "Valid: |" << c << "|" << endl;
			c = tolower(fIn.get());
			chars_read++;
			i++;
		} else {
			//cout << "2 Not Valid: |" << c << "|" << endl;
			c = tolower(fIn.get());
		}
	}

	// Count and add to context
	while (c != EOF) {
		if (!isalpha(c) && c != ' ') {
			//cout << "3 Not Valid: |" << c << "|" << endl;
			c = tolower(fIn.get());
			continue;
		}
		chars_read++;
		num_Bits += ceil(-log2(get_prob(c)));
		add_to_context(&c);
		c = tolower(fIn.get());
		//cout << "|" << c << "|" << endl;
	}
	cout << num_Bits;
	// discount EOF
	chars_read--;
	return num_Bits;
	  
}

void fcm::save_to_file(){
	count_occurrences();
	calculate_probabilities();

	if(file_out.is_open() && file_out.good()){
		boost::archive::text_oarchive oarch{file_out};
		oarch << table_probabilities;
	}
}

void fcm::load_from_file(const char *fname){
	ifstream fIn (fname);
	boost::archive::text_iarchive iarch(fIn);
	iarch >> table_probabilities;
}



//int main() {
//	fcm f(5, 0.0001, "example.txt", "file1.txt.out");
//	f.count_occurrences();
//	f.print_occurrences();
//	f.calculate_probabilities();
//	f.print_probabilities();
//	f.calculate_entropy();
//};
