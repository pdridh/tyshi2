cmake_minimum_required(VERSION 3.7)

project(tyshi2)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(app src/main.cpp)

target_link_libraries(app PRIVATE ${SDL2_LIBRARIES})
target_compile_features(app PRIVATE cxx_std_17)