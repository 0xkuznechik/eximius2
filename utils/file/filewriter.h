//  Parses a sequence of texts into separate lines. Client defines
//  the delimiter. Parsed output is returned as a vector of strings.
#ifndef UTILS_FILE_FILEWRITER_H_
#define UTILS_FILE_FILEWRITER_H_

#include <string>

#include "utils/error_handling/status.h"

namespace utils {
namespace file {
namespace filewriter {

using utils::error_handling::Status;

class FileWriter {
 public:

  static Status Open(const std::string& filepath);
  static void OpenOrDie(const std::string& filepath);
  static void WriteTo(const std::string& filepath);

};

}  // namespace filewriter
}  // namespace file 
}  // namespace utils

#endif
