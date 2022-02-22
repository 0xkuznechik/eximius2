#include "utils/sort/qsort.h"

#include "gtest/gtest.h"

namespace utils {
namespace sort {
namespace {

// TODO: Learn about allocators -.-
class QuickSortTest : public ::testing::Test {
 protected:
  void SetUp() override {
    std::vector<int> arr = {4, 3, 7, 5, 8, 9, 6, 1, 2, 0};
    QuickSort<int> sorter_(arr);
  }

  // void TearDown() override {}
};

// TODO: Implement test scaffolding + write tests
TEST_F(QuickSortTest, BasicTest) { ASSERT_EQ(1, 1); }

}  // namespace
}  // namespace sort
}  // namespace utils

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
