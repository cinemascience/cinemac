add_executable(dbtest dbtest.cxx) 
target_link_libraries(dbtest PUBLIC cinema)
target_include_directories(dbtest PUBLIC
                            "${PROJECT_SOURCE_DIR}/cinema"
                            )

add_executable(database database.cxx) 
target_link_libraries(database PUBLIC cinema)
target_include_directories(database PUBLIC
                            "${PROJECT_SOURCE_DIR}/cinema"
                            )

