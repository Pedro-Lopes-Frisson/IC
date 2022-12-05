//
// Created by whoknows on 12/5/22.
//

#ifndef P3_FCM_H
#define P3_FCM_H

#include <ostream>
#include <unordered_map>
#include <map>
#include <string>


class fcm {
private:
  int k;
  double smoothing_parameter;
  char *context;
  std::unordered_map <std::string, std::map<char, size_t>> table;


public:
  /**
   *
   * @param k order of the model
   * @param smooth_parameter avoid probabilities that are 0
   */
  fcm(int k, double smooth_parameter);
  
  /**
   *
   * @param filename  Name of the file containg a text in some languages
   * @param file_in  pointer to an ostream structure
   */
  void open_file(const char *filename, std::ostream &file_in);
  
  /**
   *
   * @param store_char  allocated pointer to store read char
   * @param f file to read from
   * @return 1 if it succeeded EOF otherwise
   */
  int get_char(char *store_char, std::ostream &f);
  
  /**
   * Builds the context
   * @param context context string
   * @param new_char new char to include in the context string
   */
  void add_to_context(const char *new_char);
  
  /**
   *
   * @param new_char Column, current character. (Map key)
   */
  void increment_counter(const char *new_char);
  
  /**
   * Calculate probabilities
   */
   void calculate_probabilities();
  
  
};


#endif //P3_FCM_H
