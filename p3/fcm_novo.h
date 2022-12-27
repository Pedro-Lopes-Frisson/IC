#include <cmath>
#include <cstddef>
#include <fstream>
#include <string>
#include <boost/serialization/map.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <vector>
#include <map>
#include <numeric>
class Fcm{
	private:
	size_t k;
	double alpha;
	std::size_t chars_read = 0;
	std::string context;
	const static std::size_t ALPHABET_LENGTH = 27; // a to z plus space
	const static std::size_t ALPHABET_START = 'a'; 

	std::map<std::size_t, std::vector<std::size_t>> mat_count;
	std::map<std::size_t, std::vector<double>> mat_prob;

	double model_entropy;

	std::ifstream file_in;
	std::ofstream file_out;

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



	void ctx_to_pos(std::string ctx,size_t *pos);
	void pos_to_ctx(size_t pos, std::string &ctx);

	public:

	/**
	 *
	 * @param k order of the model
	 * @param smooth_parameter avoid probabilities that are 0
	 * @param fIn file name containing the text to be analysed
	 * @param fOut file name to save the table probabilities matrix
	 */
	Fcm(size_t order, double_t alpha, const char *fIn, const char *fOut); // save model
	/**
	 *
	 * @param k order of the model
	 * @param smooth_parameter avoid probabilities that are 0
	 * @param fIn file name containing the text to be analysed
	 */
	Fcm(size_t order, double_t alpha, const char *fIn); // don't save model
	//
	// Load model and do something simple
	Fcm(size_t order, double_t alpha); // don't save model
	
	/**
	 * Calculate probabilities
	 */
	std::map <std::size_t, std::vector<double>> calculate_probabilities(void);

	/**
	 * Calculate entropy
	 */
	double calculate_entropy(void);


	/**
	 * Count occurrences
	 */
	void count_occurrences(void);

	// Print all data of the current model
	void print_all_matrix(void);

	/**
	 * calculate number of bits needed to encode the text according to the currently model loaded
	 */
	double calculate_nBits();

	/**
	 *  get probability of the next char being ch inside the current context inside this model
	 */
	double get_prob(char next_char);


	/**
	 *  get probability of the next char being ch inside the current context inside this model
	 */
	std::vector<double> locate_lang_ent (void);

	/**
	 *  get probability of the next char being ch inside the current context inside this model
	 */
	std::vector<double> locate_lang_nBits (void);
	/**
	 * load model saved in the file
	 */
	bool load_model(const char * fname);

	/**
	 * save model
	 */
	bool save_model(void);
};


