add_library(memalloc SHARED memalloc.h memalloc.cpp)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE fmt::fmt spdlog::spdlog memalloc)
