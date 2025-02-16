cmake_minimum_required(VERSION 3.7)

project(tyshi2)

# Set the name of the executable that comes out
set(app_name app)

# Gather source files
# Add c files cuz of shit like glad
file(GLOB_RECURSE SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.c
)


add_executable(${app_name} ${SOURCE_FILES})


# If theres a toolchain file we are auto assuming
# build for windows we dont give a fuck
if(CMAKE_TOOLCHAIN_FILE)
    message(STATUS "Building for Windows")

    set(FINAL_BIN_DIR "${CMAKE_SOURCE_DIR}/bin/win_bin")

    # MINGW requires different options so set different pkgconfig path for mingw 
    set(ENV{PKG_CONFIG_PATH} "/usr/local/x86_64-w64-mingw32/lib/pkgconfig")
    #install dlls for windows
    install(DIRECTORY libs/ DESTINATION ${FINAL_BIN_DIR})
else()
    # we auto building for linux (probably not supporting any other platforms idk)
    message(STATUS "Building for Linux/Default")
    set(FINAL_BIN_DIR "${CMAKE_SOURCE_DIR}/bin/linux_bin")
endif()

find_package(PkgConfig REQUIRED)
set(ALL_INCLUDE_DIRS)
set(ALL_LIBRARY_DIRS)
set(ALL_LIBRARIES)
set(ALL_LDFLAGS)
set(ALL_CFLAGS)

function(add_pkg PACKAGE_NAME)
    pkg_check_modules(${PACKAGE_NAME} REQUIRED ${ARGN})
    
    list(APPEND ALL_INCLUDE_DIRS ${${PACKAGE_NAME}_INCLUDE_DIRS})
    set(ALL_INCLUDE_DIRS ${ALL_INCLUDE_DIRS} PARENT_SCOPE)

    list(APPEND ALL_LIBRARY_DIRS ${${PACKAGE_NAME}_LIBRARY_DIRS})
    set(ALL_LIBRARY_DIRS ${ALL_LIBRARY_DIRS} PARENT_SCOPE)
    
    list(APPEND ALL_LIBRARIES ${${PACKAGE_NAME}_LIBRARIES})
    set(ALL_LIBRARIES ${ALL_LIBRARIES} PARENT_SCOPE)

    
    list(APPEND ALL_LDFLAGS ${${PACKAGE_NAME}_LDFLAGS})
    set(ALL_LDFLAGS ${ALL_LDFLAGS} PARENT_SCOPE)
    
    list(APPEND ALL_CFLAGS ${${PACKAGE_NAME}_CFLAGS})
    set(ALL_CFLAGS ${ALL_CFLAGS} PARENT_SCOPE)

endfunction()

add_pkg(SDL3 sdl3)
add_pkg(SDL3_IMAGE sdl3-image)
add_pkg(SDL3_TTF sdl3-ttf)


target_include_directories(app PRIVATE ${ALL_INCLUDE_DIRS})
target_link_directories(app PRIVATE ${ALL_LIBRARY_DIRS})
target_link_libraries(app PRIVATE ${ALL_LIBRARIES} -static-libstdc++ -static-libgcc)
target_link_options(app PRIVATE ${ALL_LDFLAGS})
target_compile_options(app PRIVATE ${ALL_CFLAGS})

#Compiler options
target_compile_definitions(app PRIVATE 
    $<$<CONFIG:Debug>:DEBUG_BUILD> 
    $<$<CONFIG:Release>:RELEASE_BUILD>
)

target_compile_features(${app_name} PRIVATE cxx_std_17)

install(TARGETS app RUNTIME DESTINATION ${FINAL_BIN_DIR})
file(COPY assets DESTINATION ${FINAL_BIN_DIR})
