//  Primary interface for reading from and interacting with a .sstable file.
#ifndef STORAGE_SSTABLE_SSTABLE_H_
#define STORAGE_SSTABLE_SSTABLE_H_

#include <map>
#include <string>
#include <vector>

#include "utils/error_handling/status.h"

constexpr int BUFSIZE =
    100;  // TODO: Tweak, and follow style guide recs for const

namespace storage {
namespace sstable {

class SSTable;
class SSTableIterator;

class SSTable {
 public:
  SSTable();

  utils::error_handling::Status Run();
  utils::error_handling::Status Run(const void* data);

 private:
  std::map<std::string, uint64_t> index_;
  std::map<std::string, std::string> data_blocks_;  //

  // TODO: Implement a basic file writer API
  // FileWriter writer_;
};

// Goes through data blocks in sequence
class SSTableIterator {
  // TODO
 public:
  SSTableIterator();

  utils::error_handling::Status Seek(const std::string& key);
  utils::error_handling::Status Next();

  bool done();  // returns true if end of block
  bool key();   // current key
};

}  // namespace sstable
}  // namespace storage

#endif
