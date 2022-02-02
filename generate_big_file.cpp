#include <chrono>
#include <iostream>
#include <string>

#include "file_builder.hpp"

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cerr << "Incorrect number of arguments: " << argc - 1 << "\n"
              << "There should be exactly 3 parameters: output file name, number of lines and max possible line length";
    return 0;
  }

  const std::string output_filename(argv[1]);
  const long long int lines_count = std::stoll(std::string(argv[2]));
  const int max_line_length = std::stoi(std::string(argv[3]));

  const FileBuilder file_builder(output_filename, lines_count, max_line_length);
  
  auto start_time = std::chrono::high_resolution_clock::now();
  std::cout << "Start building random file '" << output_filename << "' with " << lines_count << " lines, maximum " << max_line_length << " characters each\n";

  bool success = file_builder.BuildBigFile();

  auto stop_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop_time - start_time).count();
  
  if (success) {
    std::cout << "File written successfully!\n";
  } else {
    std::cout << "Errors occurred while writing file\n";
  }
  
  std::cout << "Time spent: " << duration << " seconds\n";

  return 0;
}
