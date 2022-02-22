#include "utils/parser/parser.h"

namespace utils {
namespace parser {

Parser::Parser(const char* ifstream, const char delimiter)
    : ifstream_(ifstream), delimiter_(delimiter){};

std::vector<std::string> Parser::Process() {
  std::vector<std::string> elements;
  std::string tmp = "";

  while (*ifstream_ != '\0') {
    while (*ifstream_ != '\0' && *ifstream_ != delimiter_) {
      tmp += *ifstream_++;
    }
    elements.push_back(tmp);
    tmp = "";
    ifstream_++;
  }
  return elements;
}

}  // namespace parser
}  // namespace utils

