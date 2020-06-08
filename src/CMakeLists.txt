cmake_minimum_required(VERSION 3.10)

# project name
project(cinema VERSION 1.0)
set (CMAKE_CXX_STANDARD 14)


# library
include_directories(${CMAKE_SOURCE_DIR}/cinema)

find_package(VTK REQUIRED)
if (VTK_FOUND)
    include(${VTK_USE_FILE})
    set (EXTRA_LIBS ${EXTRA_LIBS} ${VTK_LIBRARIES})
else()
    message(FATAL_ERROR "Could not find vtk!!!") 
endif()


# test
add_subdirectory(tests)