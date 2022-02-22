#include "storage/sstable/sstable_builder.h"

#include "gtest/gtest.h"

namespace storage {
namespace sstable {
namespace {

class SSTableBuilderTest : public ::testing::Test {
 protected:
  void SetUp() override {
		SSTableBuilder();
  }

  // void TearDown() override {}
};

TEST_F(SSTableBuilderTest, BasicTest) { ASSERT_EQ(1, 1); }

}  // namespace
}  // namespace sstable
}  // namespace storage

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

