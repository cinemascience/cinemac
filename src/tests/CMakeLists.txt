add_executable(dbtest dbtest.cxx
                      ../cinema/renderers/ospray/ArcballCamera.cpp) 
target_include_directories(dbtest PUBLIC "${PROJECT_SOURCE_DIR}/cinema")
target_link_libraries(dbtest ${PRJ_LIBS})