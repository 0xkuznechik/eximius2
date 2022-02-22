//  Parses a sequence of texts into separate lines. Client defines
//  the delimiter. Parsed output is returned as a vector of strings.
#ifndef UTILS_QSORT_QSORT_H_
#define UTILS_QSORT_QSORT_H_

#include <fstream>   // TODO: Remove after FileWriter is implemented
#include <iostream>  // DEBUG
#include <string>
#include <vector>

#include "utils/error_handling/status.h"
#include "utils/file/filewriter.h"

namespace utils {
namespace sort {

using utils::error_handling::Status;
using utils::file::filewriter::FileWriter;

template <typename T>
class QuickSort {
 public:
  QuickSort(std::vector<T> v) : v_(std::move(v)) {}

  void Sort() { Sorthelper(0, v_.size() - 1); }

  // Writes v_ to file, then empties it.
  utils::error_handling::Status WriteToFile(const std::string& filepath) {
    // TODO: Define a filewriter interface and implement.
    // Ideally, it's a library for serializing data.
    // FileWriter::WriteTo(filepath);

    std::ofstream fp(filepath);
		std::cout << "writing to " << filepath << std::endl; // DEBUG
    if (!fp) {
			std::cout << "couldn't open" << filepath << std::endl; // DEBUG
      return utils::error_handling::Status::kError;
    }

    for (auto& element : v_) {
			std::cout << "writing: " << element << std::endl;
      fp << element << std::endl;  // TODO: Does ostream make a copy?
		}

		v_.empty();
    return utils::error_handling::Status::kOk;
  }

  // DEBUG
  void Print() {
    for (const auto& i : v_) std::cout << i << " ";
    std::cout << std::endl;
  }

 protected:
  void Sorthelper(int low, int high) {
    if (low < high) {
      int p = Partition(low, high);
      Sorthelper(low, p - 1);
      Sorthelper(p + 1, high);
    }
  }

  int Partition(int low, int high) {
    T pivot_element = v_[high];
    int back_index = low - 1;

    for (int front_index = low; front_index < high; ++front_index) {
      auto& current_element = v_[front_index];
      if (current_element < pivot_element) {
        Swap(++back_index, front_index);
      }
    }
    Swap(++back_index, high);
    return back_index;
  }

  void Swap(int i, int j) {
    T tmp = std::move(v_[i]);
    v_[i] = std::move(v_[j]);
    v_[j] = std::move(tmp);
  }

 private:
  std::vector<T> v_;
};

// Factory function
template <typename T>
QuickSort<T> MakeQuickSort(std::vector<T>& v) {
  return QuickSort<T>(v);
}

}  // namespace sort
}  // namespace utils

#endif
