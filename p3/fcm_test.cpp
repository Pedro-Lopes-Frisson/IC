#include "fcm.h"

int main() {
	fcm f(2, 0.0001, "mini_example_PT.txt", "file1.txt.out");
	f.count_occurrences();
	f.print_occurrences();
	f.calculate_probabilities();
	f.calculate_entropy();
};
