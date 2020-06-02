cmake_minimum_required(VERSION 3.10)

# project name
project(cinema VERSION 1.0)

# library
add_subdirectory(cinema)

# test
enable_testing()
add_subdirectory(tests)
