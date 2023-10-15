cmake_minimum_required(VERSION 3.2)
project(shell_example)

include_directories(${_HEADERS_DIR})

set(SOURCE_FILES main.c)

add_executable(shell_example ${SOURCE_FILES})
target_link_libraries(shell_example shell)
install(TARGETS shell_example DESTINATION bin)

