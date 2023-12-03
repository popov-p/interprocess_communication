cmake_minimum_required(VERSION 3.15.0)

project(interprocess_communication)

set(CMAKE_CXX_STANDARD 14)

set(COMMON_SOURCES 
    host/host.cpp
)

set(CONNECTION_TYPES
    mmap
    shm
    pipe
)

foreach(CONNECTION_TYPE ${CONNECTION_TYPES})
    add_executable(host_${CONNECTION_TYPE}
        connections/${CONNECTION_TYPE}/conn_${CONNECTION_TYPE}.cpp
        ${COMMON_SOURCES}
    )
    add_custom_command(TARGET host_${CONNECTION_TYPE} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:host_${CONNECTION_TYPE}>
        ${CMAKE_SOURCE_DIR}
    )
endforeach()