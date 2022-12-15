//
// Created by whoknows on 12/5/22.
//

#ifndef P3_FCM_H
#define P3_FCM_H

#include <ostream>
#include <unordered_map>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>


class fcm {
private:
	int k;
	size_t chars_read = 0;
	double smoothing_parameter;
	std::vector<char> context;
	const static size_t ALPHABET_LENGTH = 27;
	const static size_t ALPHABET_START = 'a';

	std::unordered_map <std::string, std::vector<size_t>> table;
	std::unordered_map <std::string, std::vector<double>> table_probabilities;
	double model_entropy;
	// vector goes from a to z and then space

	std::ifstream file_in;
	std::ofstream file_out;


public:
	/**
	 *
	 * @param k order of the model
	 * @param smooth_parameter avoid probabilities that are 0
	 * @param fIn file name containing the text to be analysed
	 * @param fOut file name to save the table probabilities matrix
	 */
	fcm(int order, double smooth_parameter, const char *fIn, const char *fOut);

	/**
	 *
	 * @param mode If 0 it opens the file with filename to read else it opens for writing purposes
	 * @param filename  Name of the file contain a text in some languages
	 */
	void open_file(const char *filename, int mode);


	/**
	 * Builds the context
 * @param new_char new char to include in the context string
	 */
	void add_to_context(char *new_char);

	/**
	 *
	 * @param new_char Column, current character. (Map key)
	 */
	void increment_counter(const char *new_char);

	/**
	 * Calculate probabilities
	 */
	std::unordered_map <std::string, std::vector<double>> calculate_probabilities(void);

	/**
	 * Calculate entropy
	 */
	double calculate_entropy(std::unordered_map <std::string, std::vector<double>> map);
	double calculate_entropy(void);

	/**
	 * Count occurrences
	 */
	void count_occurrences(void);

	/**
	 * Print Occurrences
	 */
	void print_occurrences(void);

	/**
	 * Print probabilities
	 */
	void print_probabilities(void);

	/**
	 * Create possible contexts that can be formed with the ALPHABET(a to z)
	 * @param contexts vector to store contexts
	 * @param order length of string to be formed
	 * @param prefix initial prefix should be "", recursive function takes care of the appending to
	 * this variable
	 */

	void possible_contexts(std::vector <std::string> &contexts, int order, std::string prefix);
};


#endif //P3_FCM_H
