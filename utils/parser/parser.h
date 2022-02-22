//  Parses a sequence of texts into separate lines. Client defines
//  the delimiter. Parsed output is returned as a vector of strings.
#ifndef UTILS_PARSER_PARSER_H_
#define UTILS_PARSER_PARSER_H_

#include <string>
#include <vector>

namespace utils {
namespace parser {

class Parser {
 public:
  Parser(const char* ifstream, const char delimiter=',');

  std::vector<std::string> Process();

 private:
  const char* ifstream_;
  const char delimiter_;
};

}  // namespace parser
}  // namespace utils

#endif
