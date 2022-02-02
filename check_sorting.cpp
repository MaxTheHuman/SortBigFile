#include "is_file_sorted.hpp"

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Incorrect number of arguments: " << argc - 1 << "\n"
              << "There should be exactly 1 parameter: filename to check";
    return 0;
  }

  const std::string input_filename(argv[1]);
  bool is_file_sorted = IsFileSorted(input_filename);

  if (!is_file_sorted) {
      std::cout << "file '" << input_filename << "' isn't sorted\n";
  } else {
      std::cout << "file '" << input_filename << "' is sorted\n";
  }
  return 0;
}
