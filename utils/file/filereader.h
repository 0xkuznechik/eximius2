//  Parses a sequence of texts into separate lines. Client defines
//  the delimiter. Parsed output is returned as a vector of strings.
#ifndef UTILS_FILE_FILEREADER_H_
#define UTILS_FILE_FILEREADER_H_

#include <fstream>
#include <iostream> // DEBUG
#include <string>

#include "utils/error_handling/status.h"

namespace utils {
namespace file {
namespace filereader {

using utils::error_handling::Status;

class FileReader {
 public:

  static Status Open(const std::string& filepath) {
		std::ifstream fp(filepath);
		int len = 100;
		char buf[len];
		while(fp.getline(buf, len, '\n')) {
			std::cout << buf << std::endl;
		}
		return Status::kOk;
	};

  static void OpenOrDie(const std::string& filepath);
  static void ReadFrom(const std::string& filepath);

};

}  // namespace filereader
}  // namespace file 
}  // namespace utils

#endif
