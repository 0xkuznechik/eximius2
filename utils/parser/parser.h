#ifndef CHUNKBLOB_SSTABLE_UTILS_PARSER_H_
#define CHUNKBLOB_SSTABLE_UTILS_PARSER_H_

#include <string>
#include <vector>

namespace chunkblob {
namespace sstable {
namespace utils {

class Parser {
 public:
  Parser(const char* ifstream, const char delimiter);

  std::vector<std::string> Process();

 private:
  const char* ifstream_;
  const char delimiter_;
};

}  // namespace utils
}  // namespace sstable
}  // namespace chunkblob

#endif
