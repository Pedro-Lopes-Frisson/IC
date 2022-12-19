#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include "math.h"

using namespace std;

int main(int argc, char* argv[]) {
  // Check if the correct number of arguments was provided
  if (argc < 5) {
    cerr << "Error: Invalid number of arguments." << endl;
    cerr << "Usage: lang <ri_file> <t_file> <order> <alpha>" << endl;
    return 1;
  }

  // Read the input files and the order of the context model
  ifstream ri_file(argv[1]);
  ifstream t_file(argv[2]);
  int order = stoi(argv[3]);
  double alpha = stod(argv[4]);

  // Read the contents of the ri file into a string
  string ri_str((istreambuf_iterator<char>(ri_file)),
                    istreambuf_iterator<char>());

  // Read the contents of the t file into a string
  string t_str((istreambuf_iterator<char>(t_file)),
                     istreambuf_iterator<char>());

  // Create a map to store the counts of each context in the ri file
  unordered_map<string, int> context_counts;

  // Iterate through the ri file and count the occurrences of each context
  for (int i = 0; i < ri_str.size() - order; i++) {
    string context = ri_str.substr(i, order);
    context_counts[context]++;
  }

  // Initialize the estimated number of bits required to compress t
  double bits = 0;

  // Iterate through the t file and estimate the number of bits required to compress it
  // using the model computed from ri
  for (int i = 0; i < t_str.size() - order; i++) {
    string context = t_str.substr(i, order);
    int count = context_counts[context];
    double probability = (count + alpha) / (ri_str.size() + alpha * context_counts.size());
    bits += -log2(probability);
  }

  // Print the estimated number of bits required to compress t
  cout << "Estimated number of bits required to compress t: " << bits << endl;

  return 0;
}
