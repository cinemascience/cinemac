cmake_minimum_required(VERSION 3.10)

# project name
project(ctest VERSION 1.0)

# test
include(CTest)

# library
add_subdirectory(cinema)
add_subdirectory(tests)

# add the executable
add_executable(ctest ctest.cxx)

target_link_libraries(ctest PUBLIC cinema)

# binary tree
target_include_directories(ctest PUBLIC
                            "${PROJECT_BINARY_DIR}"
                            "${PROJECT_SOURCE_DIR}/cinema"
)

add_test( NAME dbcreate
          COMMAND ctest
        )
