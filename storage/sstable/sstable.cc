#include "storage/sstable/sstable.h"

#include <iostream> // DEBUG
#include <fstream>  // Temporarily using std
#include "utils/parser/parser.h"

namespace chunkblob {
namespace sstable {

using utils::Parser; // utils/parser.h

SSTable::SSTable(SSTableOptions& options) : options_(options) {}

utils::Status SSTable::Run() {
  std::ifstream writer(options_.filename);
  char buf[BUFSIZE];
  while (writer.getline(buf, BUFSIZE, options_.line_delimiter)) {
		Parser p(buf, options_.key_delimiter);
    auto parsed_items = p.Process();
		if (parsed_items.size() != 2) {
			return utils::Status::kError;
		}
    mapper_[parsed_items[0]] = parsed_items[1];  // TODO: Will we want to expand value?
  }

	return utils::Status::kOk;
}

utils::Status SSTable::Run(const void* data) {
	return utils::Status::kOk;
}

}  // namespace sstable
}  // namespace chunkblob
