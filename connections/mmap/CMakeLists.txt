add_library(mmap
    conn_mmap.cpp
)

target_include_directories(mmap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})