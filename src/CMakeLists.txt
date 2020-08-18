cmake_minimum_required(VERSION 3.10)

# project name
project(cinema VERSION 1.0)
set (CMAKE_CXX_STANDARD 14)


# add libs
set (PRJ_LIBS "")


#include dirs
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/cinema)
include_directories(${CMAKE_SOURCE_DIR}/cinema/renderers)
include_directories(${CMAKE_SOURCE_DIR}/cinema/utils)

set(CMAKE_CXX_FLAGS "-std=c++17")


#libraries
add_library(lodePng SHARED cinema/utils/lodepng/lodepng.h cinema/utils/lodepng/lodepng.cpp)
set (PRJ_LIBS ${PRJ_LIBS} lodePng)

option(RENDERER_OSPRAY "Enables the OSPRay renderer" ON)
if (RENDERER_OSPRAY)
    find_package(ospray 2.2.0 REQUIRED)
    if (ospray_FOUND)
        include(${OSPRAY_USE_FILE})
        add_definitions(-DHAS_OSPRAY_RENDERER)
        set (PRJ_LIBS ${PRJ_LIBS} ospray::ospray ospray::ospray_sdk ospray::ospray_testing) 
        message("Found ospray!!!") 
    else()
        message(FATAL_ERROR "Could not find ospray!!!")
    endif()
    rkcommon_configure_build_type()
endif()

option(RENDERER_VTK "Enables the VTK renderer" ON)
if (RENDERER_VTK)
    find_package(VTK REQUIRED)
    if (VTK_FOUND)
        include(${VTK_USE_FILE})
        set (EXTRA_LIBS ${EXTRA_LIBS} ${VTK_LIBRARIES})
        add_definitions(-DHAS_VTK_RENDERER)
        set (PRJ_LIBS ${PRJ_LIBS} ${VTK_LIBRARIES})
        message("Found vtk!!!") 
    else()
        message(FATAL_ERROR "Could not find vtk!!!") 
    endif()
endif()


find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

# test
add_subdirectory(tests)
