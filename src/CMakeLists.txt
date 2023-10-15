cmake_minimum_required(VERSION 3.8)

set(SOURCE_FILES
    shell.c
)

add_library(shell SHARED STATIC ${SOURCE_FILES})

install(TARGETS shell DESTINATION ${_INSTALL_LIB_DIR})
install(FILES shell.h DESTINATION ${_INSTALL_INCLUDE_DIR})
