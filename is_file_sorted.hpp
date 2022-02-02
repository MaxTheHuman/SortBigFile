#pragma once

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

bool IsFileSorted(const std::string& output_filename) {
  std::fstream file_stream;
  file_stream.open(output_filename, std::ios::in);
  if (!file_stream.is_open()) {
    std::cerr << "Can not open file for check: '" << output_filename << "'\n";
    return false;
  }

  std::string first_line, second_line;
  if (!std::getline(file_stream, first_line)) {
    std::cout << "File is empty: '" << output_filename << "'\n";
    return true;
  }

  while (std::getline(file_stream, second_line)) {
      if (second_line < first_line) {
          std::cerr << "Found not sorted lines:\n"
                    << first_line << "\n"
                    << second_line << "\n";
          return false;
      }
      first_line = second_line;
  }

  return true;
}
