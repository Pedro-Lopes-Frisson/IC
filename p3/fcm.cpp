//
// Created by whoknows on 12/5/22.
//

#include "fcm.h"
// cumulative sum
#include <numeric>
#include <iostream>

using namespace std;

fcm::fcm(int order, double smooth_parameter, const char *fIn, const char *fOut) {
  this->k = order;
  this->smoothing_parameter = smooth_parameter;
  open_file(fIn, 0);
  open_file(fOut, 1);
}

void fcm::open_file(const char *filename, int mode) {
  if (mode == 0) {
    //open file in input mode (read from)
    file_in.open(filename, ios_base::in);
    if (file_in.is_open() == 0) {
      cerr << "File open operation failed" << endl;
    }
    return;
  }
  // open file in append mode
  file_out.open(filename, ios_base::app);
}

void fcm::add_to_context(char *new_char) {
  // swap positions
  for (int i = 0; i < k - 1; ++i) {
    // n = n+1
    // current positions gets replaced by the next element
    context[i] = context[i + 1];
  }
  context[k - 1] = *new_char;
}

void fcm::increment_counter(const char *new_char) {
  // check if context exists
  if (table.find(context) == table.end()) {
    // context does not exist create it
    // create vector
    vector <size_t> new_vector(ALPHABET_LENGTH);
    
    // fill vector with default_value smoothing parameter
    // so that there are no probabilities which are zero
    fill(new_vector.begin(), new_vector.end(), smoothing_parameter);
    
    table.insert(std::pair < std::string, std::vector < size_t >> (context, new_vector));
  }
  
  // increment counter at this point is safe just to increment it
  table[context][(size_t) new_char - ALPHABET_START]++;
  return;
}

void fcm::calculate_probabilities() {
  size_t cumulative_sum;
  for (auto &entry: table) {
    //first is the context
    //second is the vector
    
    // begin of the vector, end of the vector, sum starts at smoothing_parameter instead of 0
    
    cumulative_sum = accumulate(entry.second.begin(), entry.second.end(), 0);
    
    // key did not exist create entry
    if (table_probabilities.find(entry.first) == table_probabilities.end()) {
      // context does not exist create it
      // create vector
      vector<double> new_vector(ALPHABET_LENGTH);
      // fill vector
      fill(new_vector.begin(), new_vector.end(), (double) 0);
      
      table_probabilities.insert(std::pair < std::string, std::vector < double >> (entry.first, new_vector));
    }
    
    // for each value inside vector calculate probability
    size_t cnt;
    for (size_t i = 0; i < entry.second.size(); i++) {
      // TODO: FIX FORMULA
      cnt = entry.second[i];
      table_probabilities[entry.first][i] =
          (cnt + smoothing_parameter) / (cumulative_sum + (ALPHABET_LENGTH * smoothing_parameter));
    }
    
  }
}

void fcm::print_probabilities() {
  //print header
  cout << "\t";
  for (size_t i = 0; i < ALPHABET_LENGTH; i++) {
    cout << (char) (i + ALPHABET_START) << " |";
  }
  cout << endl;
  for (auto &entry: table) {
    cout << entry.first << " | ";
    for (auto cnt: entry.second) {
      cout << cnt << " | ";
    }
  }
  cout << endl;
}

// TODO IMPLEMENT what is missing
