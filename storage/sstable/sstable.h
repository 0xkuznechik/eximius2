#ifndef STORAGE_SSTABLE_SSTABLE_H_
#define STORAGE_SSTABLE_SSTABLE_H_

#include <map>
#include <string>
#include <vector>

#include "utils/error_handling/status.h"

constexpr int BUFSIZE = 100; // TODO: Tweak, and follow style guide recs for const

namespace chunkblob {
namespace sstable {

struct SSTableOptions {
	char key_delimiter = ',';
	char line_delimiter = '\n';
	std::string filename;
	std::string output_filename;
};

class SSTable {
	public:
		SSTable(SSTableOptions& options);

		utils::Status Run();
		utils::Status Run(const void* data);

	private:
		std::map<std::string, uint64_t> index_;
		std::map<std::string, std::string> mapper_;
		SSTableOptions options_;

		// TODO: Implement a basic file writer API
		// FileWriter writer_;
};

} // sstable
} // chunkblob

#endif
