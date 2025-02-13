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
    # only have to link ts for windows 
    # mingw handles the include and shit (already set up the includes in mingw path in /usr)
    target_link_libraries(app 
        glfw3
        gdi32
    )
    target_link_options(app PRIVATE -mwindows)
else()
    # we auto building for linux (probably not supporting any other platforms idk)
    message(STATUS "Building for Linux/Default")

    # hardcoding these includes cuz this is for me and my setup also dont care
    include_directories(
        /usr/local/include
        /usr/include
    )

    # For some reason the pkg-config shit was not working and i pulled this list
    # of linking from the learnopengl website and it fucking worked so we ballin with it
    target_link_libraries(app
        glfw3
        GL
        X11
        pthread
        Xi
        dl
    )
endif()

#Compiler options
target_compile_definitions(app PRIVATE 
    $<$<CONFIG:Debug>:DEBUG_BUILD> 
    $<$<CONFIG:Release>:RELEASE_BUILD>
)

target_compile_features(${app_name} PRIVATE cxx_std_17)
