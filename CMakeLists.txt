cmake_minimum_required(VERSION 4.3.2)

project(gdl90_cxx_test CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(../gdl90 ${CMAKE_BINARY_DIR}/gdl90)

add_executable(main main.cpp)

find_package(fmt CONFIG REQUIRED)

target_link_libraries(main PRIVATE gdl90 fmt::fmt)
