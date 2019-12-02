cmake_minimum_required(VERSION 3.0)
project(noll_dbg)

add_compile_options(-std=c++17 -g -O0)

include_directories(include libelfin)
file(GLOB SOURCES "src/*.cpp")

add_executable(noll_dbg ${SOURCES})

add_custom_target(libelfin COMMAND make WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/libelfin)

add_dependencies(noll_dbg libelfin)

target_link_libraries(noll_dbg 
                      ${PROJECT_SOURCE_DIR}/libelfin/dwarf/libdwarf++.so
                      ${PROJECT_SOURCE_DIR}/libelfin/elf/libelf++.so)
