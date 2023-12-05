add_library(pipe
    conn_pipe.cpp
)

target_include_directories(pipe PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})