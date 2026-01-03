cmake_minimum_required(VERSION 3.10)
project(Cpp3DProject)

# setting the name of the executable 
set(EXECUTABLE_NAME cpp-3d)

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

# creating executable of the application
add_executable(${EXECUTABLE_NAME}
    src/Main.cc
    src/core/Window.cc
    src/core/Application.cc
    src/renderer/Renderer.cc
    src/renderer/Shaders.cc
    src/scene/Model.cc
    src/scene/Mesh.cc
    src/scene/Camera.cc
    src/input/Input.cc
    src/gui/ImguiLayer.cc
)

# imgui related
set(IMGUI_PATH ${CMAKE_SOURCE_DIR}/vendor/imgui)
file(GLOB IMGUI_SOURCES
    ${IMGUI_PATH}/*.cpp
    ${IMGUI_PATH}/backends/*.cpp
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE 
    ${IMGUI_PATH} 
    ${IMGUI_PATH}/backends
)

target_sources(${EXECUTABLE_NAME} PRIVATE ${IMGUI_SOURCES})

# include directory of the main application
include_directories(${CMAKE_SOURCE_DIR}/include)


# copying assets/ resources to the build directory
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/bugatti" 
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/shaders" 
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")


# linking libraries to the main application
target_link_libraries(${EXECUTABLE_NAME}
    PRIVATE
        glad
        assimp
        glfw
        dl
)