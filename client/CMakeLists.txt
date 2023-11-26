cmake_minimum_required(VERSION 3.15.0)

set(CMAKE_CXX_STANDARD 14)
set(CURRENT_TARGET_NAME client_executable)

set(CLIENT_SOURCE_FILES 
    client.cpp   
)

add_executable(${CURRENT_TARGET_NAME} ${CLIENT_SOURCE_FILES})

target_compile_options(${CURRENT_TARGET_NAME} PRIVATE -Wall -Werror)


add_custom_command(
    TARGET ${CURRENT_TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:${CURRENT_TARGET_NAME}>
            ${CMAKE_SOURCE_DIR}
)