add_executable(dbcreate dbcreate.cxx) 
target_link_libraries(dbcreate PUBLIC cinema)
target_include_directories(dbcreate PUBLIC
                            "${PROJECT_SOURCE_DIR}/cinema"
                            )

