#include "fcm.h"
#include <cstddef>
#include <string>
#include <iostream>

using namespace std;
int main() {
	//fcm f(18, 0.0001, "mini_example_ENG.txt", "file1.txt.out");
	fcm f(5, 0.0001, "example.txt","Model_example");
	f.count_occurrences();


	f.calculate_probabilities();
	f.calculate_entropy();

	f.save_to_file();

	fcm f2(5, 0.0001, "example.txt",NULL);
	f2.load_from_file("Model_example");


	//f.print_occurrences();
	//f.print_probabilities();
};
