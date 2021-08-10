load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = [
				"//compute:__subpackages__",
				"//storage:__subpackages__",
    ],
)

cc_library(
    name = "sstable",
    srcs = ["sstable.cc"],
    hdrs = ["sstable.h"],
		deps = [
				"//utils/parser:parser",
				"//utils/error_handling:status",
		]
)
