cmake_minimum_required(VERSION 3.18)
project(memalloc)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(memallocBUILD_TEST "是否启用测试" OFF)

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
    set(memallocBUILD_TEST ON CACHE INTERNAL "")
endif()

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_subdirectory(extern)
add_subdirectory(src)

if(${memallocBUILD_TEST})
  enable_testing()
  add_subdirectory(test)
endif()
