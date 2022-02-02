#include "file_builder.hpp"
#include "file_sorter.hpp"
#include "is_file_sorted.hpp"

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

bool TestBuildAndSort(const std::string& input_filename, int64_t lines_count, int max_line_length, int max_chunk_size) {
  const FileBuilder file_builder(input_filename, lines_count, max_line_length);
  
  std::cout << "Build file: " << input_filename << "\n";
  if (!file_builder.BuildBigFile()) {
    std::cerr << "Could not build file with name: " << input_filename << "\n";
    return false;
  }
  
  const std::string output_filename = "sorted_" + input_filename;
  const FileSorter file_sorter(input_filename, output_filename, max_chunk_size);
  file_sorter.SortBigFile();

  bool input_sorted = IsFileSorted(input_filename);
  std::cout << "Initial file sorted: " << input_sorted << "\n";
  
  if (!IsFileSorted(output_filename)) {
    std::cerr << "Result file is not sorted: " << output_filename << "\n";
    return false;
  }

  std::cout << "File is sorted: " << output_filename << "\n";

  return true;
}

int main() {
  bool test_small = TestBuildAndSort("test_small.txt", 50000, 100, 1);
  bool test_medium = TestBuildAndSort("test_medium.txt", 1000000, 100, 10);

  if (!test_small || !test_medium) {
      std::cout << "Some tests failed:\n"
                << "test_small: " << (test_small ? "Passed" : "Failed") << "\n"
                << "test_medium: " << (test_medium ? "Passed" : "Failed") << "\n";
  } else {
      std::cout << "All tests passed!\n";
  }
  return 0;
}
