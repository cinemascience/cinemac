add_executable(dbtest dbtest.cxx) 
target_include_directories(dbtest PUBLIC "${PROJECT_SOURCE_DIR}/cinema")
target_link_libraries(dbtest ${PRJ_LIBS})

add_executable(testDataset testDataset.cpp) 
target_include_directories(testDataset PUBLIC "${PROJECT_SOURCE_DIR}/cinema")
target_link_libraries(testDataset ${PRJ_LIBS})