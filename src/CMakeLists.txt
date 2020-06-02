cmake_minimum_required(VERSION 3.10)

# project name
project(test VERSION 1.0)

# library
add_subdirectory(cinema)

# add the executable
add_executable(test test.cxx)

target_link_libraries(test PUBLIC cinema)

# binary tree
target_include_directories(test PUBLIC
                            "${PROJECT_BINARY_DIR}"
                            "${PROJECT_SOURCE_DIR}/cinema"
)
