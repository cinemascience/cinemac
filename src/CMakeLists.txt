cmake_minimum_required(VERSION 3.10)

# project name
project(cinema VERSION 1.0)

# library
add_subdirectory(cinema)

# test
enable_testing()
add_subdirectory(tests)

# install
set_target_properties(cinema PROPERTIES PUBLIC_HEADER "cinema/CinBase.h;cinema/CinDBWriter.h")

install(TARGETS cinema
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include
        )
