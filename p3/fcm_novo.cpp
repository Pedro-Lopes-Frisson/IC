#include "fcm_novo.h"
#include <cmath>
#include <cstddef>
#include <iostream>
#include <vector>

using namespace std;

Fcm::Fcm(size_t order,
	 double_t alpha, const char *fIn, const char *fOut){
	k=order;
	this->alpha = alpha;
	open_file(fIn, 0);
	if(fOut != NULL)
		open_file(fOut, 1);

	clock_t start, end;

	start = clock();
	count_occurrences();
	end = clock();
	double time_taken = double(end - start) /double(CLOCKS_PER_SEC);
	cout << "Time to count ocorrences: " << time_taken << setprecision(5) << endl;

	start = clock();
	calculate_probabilities();
	end = clock();
	double time_taken2 = double(end - start) /double(CLOCKS_PER_SEC);
	cout << "Time to calculate probabilities: " << time_taken2 << setprecision(5) << endl;

	save_model();
	size_t pos;
}

Fcm::Fcm(size_t order,
	 double_t alpha, const char *fIn){
	k=order;
	this->alpha = alpha;
	open_file(fIn, 0);
	size_t pos;

}


Fcm::Fcm(size_t order,
	 double_t alpha){
	k=order;
	this->alpha = alpha;
	size_t pos;
}

// a -> 0
//
void Fcm::open_file(const char *filename, int mode) {
	if (mode == 0) {
		//open file in input mode (read from)
		file_in.open(filename, ios_base::in);
		if (!file_in.is_open()) {
			cerr << "File open operation failed; " << filename << endl;
		}
		return;
	}
	// open file in append mode
	file_out.open(filename, ios_base::trunc);
}

void Fcm::ctx_to_pos(string ctx, size_t *pos){
	// a -> 0
	// b -> 1
	// aa -> 0 + 1 -> 0 + 0 0 + 1
	// aaa -> 0 + 1 + -> 0 + 0 0 + 1
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

void Fcm::pos_to_ctx(size_t pos, std::string &ctx){
	size_t ch;
	for (int i = 0; i < k; i++) {
		ch = ((unsigned int) (pos / pow(ALPHABET_LENGTH, i)) % ALPHABET_LENGTH);
		ctx += (ch != 26) ? ch + ALPHABET_START : ' ';
		pos -= ch * pow(ALPHABET_LENGTH, i);
    }
}

void Fcm::add_to_context(char *new_char) {
	if (context.size() > k - 1) {
		context.erase(context.begin());
	}
	context += *new_char;
}

void Fcm::increment_counter(const char *new_char) {
	size_t key;
	ctx_to_pos(context,&key);

	if (mat_count.count(key) == 0) {
		// context does not exist create it
		// create vector
		vector <size_t> new_vector(ALPHABET_LENGTH);

		fill(new_vector.begin(), new_vector.end(), 0);
		mat_count.insert(pair < size_t,
		             vector < size_t >> (key, new_vector));

	}
	// increment counter at this point is safe just to increment it
	if (mat_count.find(key)->second.size() == 0) {
		vector <size_t> new_vector(ALPHABET_LENGTH);

		fill(new_vector.begin(), new_vector.end(), 0);
		mat_count.erase(key);
		
		mat_count.insert(pair < size_t,
		             vector < size_t >> (key, new_vector));
	}
	if (*new_char != ' ')
		mat_count[key][(size_t)(*new_char) - ALPHABET_START]++;
	else
		mat_count[key][ALPHABET_LENGTH - 1]++; // space
	return;
}

map <size_t, vector<double>> Fcm::calculate_probabilities() {
	size_t cumulative_sum;
	for(auto &entry : mat_count){
		cumulative_sum = accumulate(entry.second.begin(), entry.second.end(),
		                            0);

		if (mat_prob.find(entry.first) == mat_prob.end()) {
			mat_prob.insert(pair<size_t, vector<double>>(entry.first, vector<double>(ALPHABET_LENGTH, alpha / (ALPHABET_LENGTH * alpha))));
		}
		double pb, paeb;
		// for each value inside vector calculate probability
		size_t cnt;
		for (size_t j = 0; j < entry.second.size(); j++) {
			cnt = entry.second[j];
			pb = cumulative_sum / (double) chars_read;
			paeb = (cnt + alpha) /
			       (cumulative_sum + (ALPHABET_LENGTH * alpha));

			mat_prob[entry.first][j] = (cnt + alpha)
			                                      / (cumulative_sum +
			                                         (ALPHABET_LENGTH * alpha));
		}
	}

	return mat_prob;
}



double Fcm::calculate_entropy() {
	if(mat_count.size() == 0) {
		cerr << "You did not use count Occurrences" << endl;
		return 0.0;
	}
		
	// TODO: FIX THIS
	model_entropy = 0;
	double ctx_entropy = 0;
	double prob;
	double cum_sum = 0;
	vector<double> vc_default(ALPHABET_LENGTH, alpha);
	vector<double> vp_default(ALPHABET_LENGTH, alpha / (alpha * ALPHABET_LENGTH));

	for (auto &entry: mat_prob) {
		ctx_entropy = 0;
		for (size_t i = 0; i < entry.second.size(); i++) {
			// if count was zero then don't use it to calculate the entropy
			//if (mat_count.find(entry.first) == mat_count.end()) continue;
			prob = entry.second[i];

			ctx_entropy += -(prob * log2(prob));
		}
		cum_sum = accumulate(mat_count[entry.first].begin(), mat_count[entry.first].end(), 0);
		model_entropy += (cum_sum / chars_read) * ctx_entropy;
	}
	//cout << "Entropy: " << model_entropy << endl;

	return model_entropy;
}

bool Fcm::save_model(){

	count_occurrences();
	calculate_probabilities();

	if(file_out.is_open() && file_out.good()){
		boost::archive::text_oarchive oarch{file_out};
		oarch << mat_prob;
		return 1;
	}

	return 0;
}

bool Fcm::load_model(const char *fname){
	ifstream fIn (fname);
	boost::archive::text_iarchive iarch(fIn);
	iarch >> mat_prob;
	return 1;
}

void Fcm::print_all_matrix(){

	cout << "mat_count OF OCCURRENCES" << endl;
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
		if (mat_count.count(i) == 0){
			// entry does not exist
			mat_count.insert(pair<size_t, vector<size_t>> (i, vector<size_t>(ALPHABET_LENGTH, 0)));
		}
		auto &entry = *mat_count.find(i);
		pos_to_ctx(entry.first, s);
		cout << " | " << s << " |";
		for (auto prob: entry.second)
			cout << " " << prob << " |";
		cout << endl;
		s.clear();
	}
	cout << endl;

	cout << "mat_count OF PROBABILITIES" << endl;
	cout << "| Context/Symbol | ";
	for (size_t i = 0; i < ALPHABET_LENGTH; i++) {
		if (i == ALPHABET_LENGTH - 1)
			cout << "<SPACE>|";
		else
			cout << (char) (ALPHABET_START + i) << "  |";
	}
	cout << endl;
	for (size_t i = 0; i < pow(ALPHABET_LENGTH, k); i++) {
		if (mat_prob.count(i) == 0){
			// entry does not exist
			vector<double> new_vector(ALPHABET_LENGTH);
			// fill vector
			fill(new_vector.begin(), new_vector.end(), (double) 0);

			mat_prob.insert(std::pair < size_t, vector < double>> (i , new_vector));
		}
		auto &entry = *mat_prob.find(i);
		pos_to_ctx(entry.first, s);
		cout << "| " << s << "           |";
		for (auto prob: entry.second)
			cout << " " << prob << " |";
		cout << endl;
	}
	cout << endl;
}

double Fcm::get_prob(char next_char){
	size_t key;
	ctx_to_pos(context, &key);
	if (mat_prob.find(key) == mat_prob.end()) {
		return alpha / (alpha * ALPHABET_LENGTH);
	}
	if(next_char == ' ')
		return mat_prob[key][ALPHABET_LENGTH - 1];
	return mat_prob[key][ (size_t)next_char - ALPHABET_START];
}

double Fcm::calculate_nBits(){
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
			//chars_read++;
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
		//chars_read++;
		prob = get_prob(c);

		num_Bits += ceil(-log2(get_prob(c)));
		add_to_context(&c);
		c = tolower(file_in.get());
		//cout << "|" << c << "|" << endl;
	}
	//cout << "Num Bits: " << num_Bits << endl;
	// discount EOF
	chars_read--;
	return num_Bits;
}



void Fcm::count_occurrences() {

	file_in.clear();
	file_in.seekg(0);

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


vector<double> Fcm::locate_lang_nBits(size_t bs) {
	const size_t save_bs = bs;
	file_in.clear();
	file_in.seekg(0);
	vector<double> nBits_values;

	int i = 0;
	char c;
	// read first char
	c = tolower(file_in.get());
	//cout << "First: |" << c << "|" << endl;
	chars_read++;
	double prob;
	size_t num_Bits = 0;

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
		//chars_read++;
		prob = get_prob(c);

		if( bs == 0){
			bs = save_bs;
			num_Bits = 0;
		}

		//num_Bits += ceil(-log2(get_prob(c)));
		//nBits_values.push_back(num_Bits);
		nBits_values.push_back(ceil(-log2(get_prob(c))));
		add_to_context(&c);
		c = tolower(file_in.get());
		bs--;
		//cout << "|" << c << "|" << endl;
	}

	// discount EOF
	chars_read--;
	return nBits_values;
}
