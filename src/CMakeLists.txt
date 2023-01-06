add_library(datalogger SHARED)
target_include_directories(
    datalogger 
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
target_sources(
    datalogger
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/datalogger.cc
)
target_link_libraries(datalogger PRIVATE Threads::Threads)