load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = [
		"//compute:__subpackages__",
		"//storage:__subpackages__",
		],
)

cc_library(
		name = "data",
		srcs = ["data.h"],
)

cc_library(
		name = "sstable",
		srcs = ["sstable.cc"],
		hdrs = ["sstable.h"],
		deps = [
				"//utils/parser:parser",
				"//utils/error_handling:status",
		],
)

cc_library(
		name = "sstable_builder",
		srcs = ["sstable_builder.cc"],
		hdrs = ["sstable_builder.h"],
		deps = [
				":sstable",
				":data",
				"//utils/error_handling:status",
				"//utils/file:filewriter",
				"//utils/file:filereader",
				"//utils/sort:qsort",
				"@com_google_absl//absl/strings",
		],
)

# TODO: Remove after done, this was for testing
cc_binary(
		name = "main",
		srcs = ["sstable_main.cc"],
		deps = [
				":sstable_builder",
		],
)

cc_test(
  name = "sstable_builder_test",
  size = "small",
  srcs = ["sstable_builder_test.cc"],
  deps = [
			":sstable_builder",
			"@com_google_googletest//:gtest_main",
	],
)
