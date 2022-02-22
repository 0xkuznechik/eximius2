#include <iostream>

#include "storage/sstable/sstable_builder.h"

int main(int argc, char **argv) {
  std::string fname = "example";
	{
		storage::sstable::SSTableBuilder b(fname);
		b.Add("a", "b");
		b.Add("x", "y");
		b.Add("1", "2");
	}

	// storage::sstable::KeyValue kv{"a", "b"};
	// std::cout << kv << std::endl;

  return 0;
}
