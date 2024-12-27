cmake_minimum_required(VERSION 3.7)

project(tyshi2)

set(app_name app)

# Gather source files
file(GLOB_RECURSE SOURCE_FILES 
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

add_executable(${app_name} ${SOURCE_FILES})


include(FindPkgConfig)

pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
target_link_libraries(${app_name} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
target_compile_features(${app_name} PRIVATE cxx_std_17)