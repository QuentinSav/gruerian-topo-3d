# Set Name of project and language
project(gruerian-topo-3d LANGUAGES CXX C)

# Set cmake version
cmake_minimum_required(VERSION 3.12)

# Set build type to Debug/Release
set(CMAKE_BUILD_TYPE "Debug")

# Optionally, add external libraries and link them to your project
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GDAL REQUIRED)

# Include directories for glad.h and your project source files
include_directories(${PROJECT_SOURCE_DIR}/libs/glad)
include_directories(${PROJECT_SOURCE_DIR}/libs/stb)
include_directories(${PROJECT_SOURCE_DIR}/libs/glm)
include_directories(${PROJECT_SOURCE_DIR})

# Create executable using the specified src
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/controler/InputController.cpp
    src/model/Outing.cpp
    src/model/Texture.cpp
    src/model/Mesh.cpp
    src/model/Light.cpp
    src/model/Skybox.cpp
    src/model/MeshDEM.cpp
    src/view/Renderer.cpp
    src/util/Shader.cpp
    src/util/Camera.cpp
    ${PROJECT_SOURCE_DIR}/libs/glad/glad.c
    ${PROJECT_SOURCE_DIR}/libs/stb/stb_image.cpp
)

# Define required C++ standard to C++11
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)

# Specify the include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# Set compile options, enable warnings
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>: -Wall>
    $<$<CXX_COMPILER_ID:MSVC>: /W3>
)

# Ensure glad.c is compiled as C code
set_source_files_properties(${PROJECT_SOURCE_DIR}/libs/glad/glad.c PROPERTIES LANGUAGE C)

# Link GLFW, OpenGL and GDAL libraries
target_link_libraries(${PROJECT_NAME} PRIVATE glfw OpenGL::GL)
target_link_libraries(${PROJECT_NAME} PRIVATE GDAL::GDAL)