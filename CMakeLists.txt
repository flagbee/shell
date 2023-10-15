cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project(shell)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

#project ("shell")
#kadd_subdirectory ("shell")
set(_INSTALL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(_INSTALL_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(_INSTALL_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(_HEADERS_DIR ${PROJECT_SOURCE_DIR}/src)

include_directories(${_INSTALL_INCLUDE_DIR})
include_directories(${_HEADERS_DIR})

add_subdirectory(src)
add_subdirectory(example)

