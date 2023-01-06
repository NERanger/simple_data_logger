add_executable(sample 
    ${CMAKE_CURRENT_LIST_DIR}/sample.cc
)
target_link_libraries(sample PRIVATE datalogger)