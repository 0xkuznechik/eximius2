#include "storage/sstable/sstable.h"

#include <fstream>   // Temporarily using std
#include <iostream>  // DEBUG

#include "utils/parser/parser.h"

namespace storage {
namespace sstable {

using utils::parser::Parser;  // utils/parser.h

SSTable::SSTable() {}

utils::error_handling::Status SSTable::Run() {
  /*
std::ifstream writer(options_.filename);
char buf[BUFSIZE];
while (writer.getline(buf, BUFSIZE, options_.line_delimiter)) {
Parser p(buf, options_.key_delimiter);
auto parsed_items = p.Process();
if (parsed_items.size() != 2) {
return utils::error_handling::Status::kError;
}
data_blocks_[parsed_items[0]] =
  parsed_items[1];  // TODO: Will we want to expand value?
}
  */
  return utils::error_handling::Status::kOk;
}

utils::error_handling::Status SSTable::Run(const void* data) {
  return utils::error_handling::Status::kOk;
}

}  // namespace sstable
}  // namespace storage
