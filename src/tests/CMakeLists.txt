add_executable(dbtest dbtest.cxx) 
target_include_directories(dbtest PUBLIC "${PROJECT_SOURCE_DIR}/cinema")
target_link_libraries(dbtest ${VTK_LIBRARIES})

add_executable(database database.cxx) 
target_include_directories(database PUBLIC "${PROJECT_SOURCE_DIR}/cinema")
target_link_libraries(dbtest ${VTK_LIBRARIES})