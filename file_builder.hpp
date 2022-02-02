#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class FileBuilder {
public:
  FileBuilder(const std::string& output_filename, int64_t lines_count, int max_line_length)
    : output_filename_(output_filename),
      lines_count_(lines_count),
      max_line_length_(max_line_length) {}

  bool BuildBigFile() const {
    std::fstream output_file_stream;
    output_file_stream.open(output_filename_, std::ios::out);

    if (!output_file_stream.is_open()) {
      std::cerr << "Can not open file with name '" << output_filename_ << "' for writing\n";
      return false;
    }

    try {
      std::cout << "Progress: ";
      for (long long int i = 0; i < lines_count_; ++i) {
        if (i % (lines_count_ / 100 + 1) == 0) {
          std::cout << (static_cast<double>(i) / lines_count_) * 100 << " %" << std::endl;
        }
        int current_line_length = std::rand() % max_line_length_ + 1;
        std::stringstream current_line_stream("");
        for (int j = 0; j < current_line_length; ++j) {
          current_line_stream << template_string[std::rand() % 62];
        }
        current_line_stream << "\n";
        output_file_stream << current_line_stream.str();
      }
    } catch (const std::exception& ex) {
      std::cerr << "Exception caught while writing file: " << ex.what() << "\n";
      output_file_stream.close();
      return false;
    }

    output_file_stream.close();
    return true;
  }

private:
  const std::string output_filename_;
  const int64_t lines_count_;
  const int max_line_length_;

  const std::string template_string = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
};