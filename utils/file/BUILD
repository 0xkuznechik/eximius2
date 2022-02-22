load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = [
				"//compute:__subpackages__",
				"//storage:__subpackages__",
				"//utils:__subpackages__",
    ],
)

cc_library(
    name = "filewriter",
    srcs = ["filewriter.cc"],
    hdrs = ["filewriter.h"],
		deps = [
				"//utils/error_handling:status",
		],
)

cc_library(
    name = "filereader",
    srcs = ["filereader.cc"],
    hdrs = ["filereader.h"],
		deps = [
				"//utils/error_handling:status",
		],
)
