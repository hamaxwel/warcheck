cmake_minimum_required(VERSION 3.10)
project(InvasionEngine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Set dependencies path
set(DEPS_DIR ${CMAKE_SOURCE_DIR}/dependencies)

# Add GLEW paths
set(GLEW_ROOT_DIR "${DEPS_DIR}/glew/glew-2.2.0")
set(GLEW_INCLUDE_DIRS "${GLEW_ROOT_DIR}/include")
set(GLEW_LIBRARIES "${GLEW_ROOT_DIR}/lib/Release/x64/glew32.lib")

# Add OpenAL paths
set(OPENAL_ROOT_DIR "${DEPS_DIR}/openal")
set(OPENAL_INCLUDE_DIR "${OPENAL_ROOT_DIR}/include")
set(OPENAL_LIBRARY "${OPENAL_ROOT_DIR}/libs/Win64/OpenAL32.lib")

# Add GLFW paths
set(GLFW_ROOT_DIR "${DEPS_DIR}/glfw")
set(GLFW_INCLUDE_DIRS "${GLFW_ROOT_DIR}/include")
set(GLFW_LIBRARIES "${GLFW_ROOT_DIR}/lib-vc2022/glfw3.lib")

# Add GLM paths
set(GLM_ROOT_DIR "${DEPS_DIR}/glm/glm")
set(GLM_INCLUDE_DIRS "${GLM_ROOT_DIR}")

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(jsoncpp REQUIRED)

# Include directories
include_directories(
    ${GLEW_INCLUDE_DIRS}
    ${OPENAL_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${JSONCPP_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/Source
)

# Link directories
link_directories(
    ${DEPS_DIR}/lib
)

# Add source files
file(GLOB_RECURSE SOURCES 
    "Source/*.cpp"
    "Source/*.h"
)

# Create executable
add_executable(InvasionEngine ${SOURCES})

# Include directories
target_include_directories(InvasionEngine PRIVATE
    ${CMAKE_SOURCE_DIR}/Source
    ${OPENGL_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${OPENAL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIRS}
    ${JSONCPP_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(InvasionEngine PRIVATE
    ${GLEW_LIBRARIES}
    ${OPENAL_LIBRARY}
    ${GLFW_LIBRARIES}
    ${JSONCPP_LIBRARIES}
    opengl32
)

# Copy DLLs to output directory
add_custom_command(TARGET InvasionEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${DEPS_DIR}/bin
    $<TARGET_FILE_DIR:InvasionEngine>
)

# Copy assets to build directory
file(COPY ${CMAKE_SOURCE_DIR}/Assets DESTINATION ${CMAKE_BINARY_DIR})

# Set compiler flags
if(MSVC)
    target_compile_options(InvasionEngine PRIVATE /W4)
else()
    target_compile_options(InvasionEngine PRIVATE -Wall -Wextra -Wpedantic)
endif() 