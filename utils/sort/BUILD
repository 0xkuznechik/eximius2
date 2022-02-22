load("@rules_cc//cc:defs.bzl", "cc_library")
load("@rules_cc//cc:defs.bzl", "cc_test")
load("@rules_cc//cc:defs.bzl", "cc_binary")

package(
    default_visibility = [
				"//compute:__subpackages__",
				"//storage:__subpackages__",
				"//utils:__subpackages__",
    ],
)

cc_library(
    name = "qsort",
    # srcs = ["qsort.cc"],
    hdrs = ["qsort.h"],
		deps = [
				"//utils/error_handling:status",
				"//utils/file:filewriter",
		],
)

cc_binary(
    name = "main",
    srcs = ["main.cc"],
		deps = [
				":qsort",
		],
)


cc_test(
  name = "qsort_test",
  size = "small",
  srcs = ["qsort_test.cc"],
  deps = [
			":qsort",
			"@com_google_googletest//:gtest_main",
	],
)

