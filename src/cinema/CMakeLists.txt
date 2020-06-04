add_library(cinema 
                CinBase.cxx CinBase.h
                CinDatabase.cxx CinDatabase.h
                CinDBWriter.cxx CinDBWriter.h
                )

find_package(VTK REQUIRED NO_MODULE)

target_link_libraries(cinema ${VTK_LIBRARIES})
