#include "storage/sstable/sstable_builder.h"

#include <iostream>

#include "absl/strings/str_cat.h"
#include "utils/file/filewriter.h"
#include "utils/file/filereader.h"
#include "utils/sort/qsort.h"

namespace storage {
namespace sstable {

const std::string DEFAULT_FILEPATH = "default";

using utils::file::filewriter::FileWriter;
using utils::file::filereader::FileReader;

SSTableBuilder::SSTableBuilder()
    : filepath_(DEFAULT_FILEPATH), options_({}), index_count_(0) {}

SSTableBuilder::SSTableBuilder(std::string& filepath, SSTableOptions options)
    : filepath_(std::move(filepath)),
      options_(std::move(options)),
      index_count_(0) {
  // FileWriter::OpenOrDie(filepath_);
}

SSTableBuilder::~SSTableBuilder() {
	QuickSortAndWrite();
  MergeFiles();
  // FileWriter::Close(filepath_);
}

// DEBUG
void SSTableBuilder::Test() {}

Status SSTableBuilder::Add(std::string key, std::string value) {
  KeyValue kv{key, value};

  if (++index_count_ > BUF_SIZE) {
    QuickSortAndWrite();
  }

  buffer_.push_back(std::move(kv));
  return utils::error_handling::Status::kOk;
}

Status SSTableBuilder::QuickSortAndWrite() {
  // O(N*LOGN): Sort buffer_ by {key, value},
  // TODO: Make sure this isn't copying?
	auto q = utils::sort::MakeQuickSort(buffer_);  // factory function with type deduction
	q.Sort();


	// TODO: Implement an ASSERT to check filename
	std::string writeto_filename = absl::StrCat(filepath_, index_count_++);
	tmp_files_.push_back(writeto_filename);
	q.WriteToFile(writeto_filename); // Empties buffer as well
	buffer_.clear();

  // TODO
  // O(N) : Write directly into a temporary file on disk (.sstable); keep
  // track of these files.
  return utils::error_handling::Status::kOk;
}

Status SSTableBuilder::MergeFiles() {
  // TODO: Merge sort all the temporary files into a single one, then clean up.
	// This will require splitting up the files into concurrent pairs, then
	// tracking the temporary files each pair is merged into, and repeating this
	// process again until we only have a single file.
	// 1) Concurrency.
	// 2) Concurrency with merge sorting an array.
	// 3) Concurrency with opening and closing files.
	// 4) This task.
	for (const auto& filename : tmp_files_) {
		std::cout << "filename: " << filename << std::endl;
		FileReader::Open(filename);
	}
  return utils::error_handling::Status::kOk;
}

}  // namespace sstable
}  // namespace chunkblob
