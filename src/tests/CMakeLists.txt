add_executable(dbcreate dbcreate.cxx) 
add_test( dbcreate dbcreate )

target_link_libraries(dbcreate PUBLIC cinema)

target_include_directories(dbcreate PUBLIC
                            "${PROJECT_SOURCE_DIR}/cinema"
                            )

