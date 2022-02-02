#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>

namespace {
  const int64_t kBytesInMegaByte = 1 << 20;  // 2^20
}

class FileSorter {
public:
  FileSorter(const std::string& input_filename, const std::string& output_filename, int max_chunk_size)
    : input_filename_(input_filename),
      output_filename_(output_filename),
      max_chunk_size_(max_chunk_size) {}

  void SortBigFile() const {
    std::fstream input_file_stream;
    input_file_stream.open(input_filename_, std::ios::in | std::ios::out);
    
    if (!input_file_stream.is_open()) {
      std::cerr << "Can not open file with name '" << input_filename_ << "' for reading and writing\n";
      return;
    }

    std::fstream output_file_stream;
    output_file_stream.open(output_filename_, std::ios::out);
    if (!output_file_stream.is_open()) {
      std::cerr << "Can not open file with name '" << output_filename_ << "' for writing the result\n";
      return;
    }

    try {
      
      std::string str_holder;
      std::vector<std::string> strings_chunk;
      std::vector<std::streampos> blocks_start_pos;

      blocks_start_pos.push_back(input_file_stream.tellp());

      const auto input_file_one_percent_pos = FindOnePercentCharsOfFile(input_file_stream);
      int current_percent_sorted = 0;

      std::cout << "Sort chunks\nProgress:\n";
      while (std::getline(input_file_stream, str_holder)) {
        if (input_file_stream.tellg() > (current_percent_sorted + 1) * input_file_one_percent_pos) {
          current_percent_sorted = input_file_stream.tellg() / input_file_one_percent_pos;
          std::cout << current_percent_sorted << " %" << std::endl;
        }
        strings_chunk.push_back(str_holder);
        const auto prev_pos = blocks_start_pos.back();
        if (input_file_stream.tellg() - prev_pos > max_chunk_size_ * kBytesInMegaByte) {
          SortStringsChunkAndWriteToFile(strings_chunk, input_file_stream, prev_pos);
          blocks_start_pos.push_back(input_file_stream.tellp());
        }
      }
    
      input_file_stream.clear();
      SortStringsChunkAndWriteToFile(strings_chunk, input_file_stream, blocks_start_pos.back());

      const auto blocks_start_pos_original = blocks_start_pos;

      auto comp = [](
        const std::pair<size_t, std::string>& left,
        const std::pair<size_t, std::string>& right) {
          return left.second > right.second;
        };
      std::priority_queue<
        std::pair<size_t, std::string>,
        std::vector<std::pair<size_t, std::string>>,
        decltype(comp)> strings_heap(comp);

      // fulfill our heap
      for (size_t i = 0; i < blocks_start_pos.size(); ++i) {

        input_file_stream.seekp(blocks_start_pos[i]);
        
        if (std::getline(input_file_stream, str_holder)) {
          strings_heap.push(std::make_pair(i, str_holder));
          blocks_start_pos[i] = input_file_stream.tellp();
        } else {
          input_file_stream.clear();
          blocks_start_pos[i] = 0;
        }
      }

      current_percent_sorted = 0;
      std::cout << "Chunks sorted, merge them into result file\nProgress:\n";

      // start k-merge sort
      while (!strings_heap.empty()) {
        const auto curr_line = strings_heap.top();
        // std::cout << "curr_line = (" << curr_line.first << ", " << curr_line.second << ">\n";
        strings_heap.pop();
        output_file_stream << curr_line.second << "\n";
        if (output_file_stream.tellp() > (current_percent_sorted + 1) * input_file_one_percent_pos) {
          current_percent_sorted = output_file_stream.tellp() / input_file_one_percent_pos;
          std::cout << current_percent_sorted << " %" << std::endl;
        }

        input_file_stream.seekp(blocks_start_pos[curr_line.first]);
        if (std::getline(input_file_stream, str_holder)
            && (curr_line.first == blocks_start_pos_original.size() - 1
                || blocks_start_pos[curr_line.first] !=
                   blocks_start_pos_original[curr_line.first + 1])) {
          strings_heap.push(std::make_pair(curr_line.first, str_holder));
          blocks_start_pos[curr_line.first] = input_file_stream.tellp();
        } else {
          input_file_stream.clear();
          blocks_start_pos[curr_line.first] = 0;
        }
      }

    } catch (const std::exception& ex) {
      std::cerr << "Exception caught while reading and writing file: " << ex.what() << "\n";
      input_file_stream.close();
      output_file_stream.close();
      return;
    }

    input_file_stream.close();
    output_file_stream.close();
    std::cout << "Sorting done\n";
  }

private:
  std::streampos FindOnePercentCharsOfFile(std::fstream& input_file_stream) const {
    input_file_stream.seekg(0, std::ios::end);
    std::streampos end_pos = input_file_stream.tellg();
    input_file_stream.seekg(0);
    return end_pos / 100;
  }

  void SortStringsChunkAndWriteToFile(
      std::vector<std::string>& strings_chunk,
      std::fstream& input_file_stream,
      std::streampos prev_pos) const {

    std::sort(strings_chunk.begin(), strings_chunk.end());

    input_file_stream.seekp(prev_pos);
    for (const auto& line : strings_chunk) {
        input_file_stream << line << "\n";
    }

    strings_chunk.clear();
  }

  const std::string input_filename_;
  const std::string output_filename_;
  const int max_chunk_size_;
};
