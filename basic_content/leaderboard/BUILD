# please run `bazel run basic_content/leaderboard:main`
load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

# example - h
# cc_library(
#     name = "inline",
#     hdrs = ["inline.h"],
# )

# example - cpp
# cc_binary(
#     name = "inline_virtual",
#     srcs = ["inline_virtual.cpp"],
#     copts = ["-std=c++11"]
# )

# cc_binary(
#     name = "main",
#     srcs = ["inline.cpp"],
#     deps = [
#         ":inline",
#     ],
#     copts = ["-std=c++11"]
# )

cc_binary(
    name = "main",
    srcs = ["list_unordered_map.cpp"],
    copts = ["-std=c++17"]
)