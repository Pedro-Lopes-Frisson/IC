#include "fcm.h"

int main() {
  fcm fcm{1, 10, "test_file", "___"};
  fcm.count_occurrences();
  fcm.print_occurrences();
  return 0;
}
