add_library(cinema 
                CinBase.cxx CinBase.h
                CinDBWriter.cxx CinDBWriter.h
                )

set(VTK_DIR "/Users/VTK")
find_package(VTK REQUIRED)

target_link_libraries(cinema ${VTK_LIBRARIES})
