cmake_minimum_required(VERSION 3.10)
project(Cpp3DProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(assimp REQUIRED)

# Set the build type to Debug
set(CMAKE_BUILD_TYPE Debug)

# -g: adds debug symbols
# -Wall: enables all common warnings 
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# adding glad library as a static library
add_library(glad STATIC 
    glad_generated/src/gl.c
)
# Tell CMake where GLAD's headers are
target_include_directories(glad PUBLIC glad_generated/include)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/bugatti" 
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/shaders" 
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

add_executable(cpp-3d
    src/Main.cc
    src/Model.cc
    src/Mesh.cc
    src/Shaders.cc
    src/Camera.cc
)

target_link_libraries(cpp-3d
    PRIVATE
        glad
        assimp
        glfw
        dl
)