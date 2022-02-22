//  Builds an SSTable file (.sstable).
//
//  For now, implementation is higher level, relying on the C++ compiler's local
//  memory model and APIs.
//
//  DATA STRUCTURE
//
//	Each Key-Value pair will be a struct, stored in memory. An index to the
//	address of each KV pair is stored in a vector.
//
//  ADDING DATA
//
//	Adding KV pairs - create a new struct, perform an insert to a
//  quick-sorted array of indexes by key. If the same key is found, perform
//  a quicksort insert to array of indexes by value for that same key.
//  For now, proof-of-concept implementation, make a std::map of std::string to
//  std::vector of memory addresses for each key.
//
//  MERGING DATA
//
//	When the Builder's destructor is called, then write to .sstable file on
//	disk. This is done by taking all the tmp files and merging them.
//	Note: As of now, this is done in a very stupid and inefficient way. It will
//	perform a merge sort on the files' contents. This will require reading two
//	files and writing into another one, then reading from that merged tmp file
//	again, and repeating this process until we have a single file (the final
//	.sstable file). This can possibly be made faster with concurrency.
//
//	Future consideration: scanning over each file's header/first data element
//	and looking for the lowest sorted value, then "popping" it off into the
//	final .sstable file, which is open.
//
//  TODO: If destructor is never called, will this hang??

#ifndef STORAGE_SSTABLE_SSTABLE_BUILDER_H_
#define STORAGE_SSTABLE_SSTABLE_BUILDER_H_

#include <iostream> // Used to overload ostream for struct KeyValue
#include <map>
#include <string>
#include <vector>

#include "storage/sstable/data.h"
#include "utils/error_handling/status.h"

constexpr int BUF_SIZE = 2; // num kv items

namespace storage {
namespace sstable {

using storage::sstable::KeyValue;
using utils::error_handling::Status;

struct SSTableOptions {
  char key_delimiter = ',';
  char line_delimiter = '\n';
  std::string filepath;  // Location of where to save .sstable
  std::string filename;  // Filename of .sstable; could be sharded
};

class SSTableBuilder;

class SSTableBuilder {
 public:
  SSTableBuilder();
  SSTableBuilder(std::string& filepath,  // Where to store file
                 SSTableOptions options = {});
  ~SSTableBuilder();

  Status Add(std::string key, std::string value);
  Status QuickSortAndWrite();
	Status MergeFiles();

	// DEBUG
  void Test();

 private:
  const std::string filepath_;
  std::map<std::string, std::vector<uint64_t>> index_;
  std::map<std::string, std::vector<KeyValue>> data_blocks_;
  SSTableOptions options_;

  int index_count_;
	std::vector<KeyValue> buffer_;
	std::vector<std::string> tmp_files_;

  // TODO: Implement a basic file writer API
  // FileWriter writer_;
};

}  // namespace sstable
}  // namespace chunkblob

#endif
