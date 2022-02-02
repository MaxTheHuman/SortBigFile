#include <chrono>
#include <iostream>
#include <string>

#include "file_sorter.hpp"

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cerr << "Incorrect number of arguments: " << argc - 1 << "\n"
              << "There should be exactly 3 parameters: input file name, output file name and max chunk in Kb";
    return 0;
  }

  const std::string input_filename(argv[1]);
  const std::string output_filename(argv[2]);
  const int max_chunk_size = std::stoi(std::string(argv[3]));

  const FileSorter file_sorter(input_filename, output_filename, max_chunk_size);
  
  auto start_time = std::chrono::high_resolution_clock::now();
  std::cout << "Start sorting file '" << input_filename << "', max chunk size in Mb: " << max_chunk_size << "\n";

  file_sorter.SortBigFile();

  auto stop_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop_time - start_time).count();
  
  std::cout << "Time spent: " << duration << " seconds\n";

  return 0;
}
