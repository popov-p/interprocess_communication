add_library(shm
    conn_shm.cpp
)

target_include_directories(shm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})