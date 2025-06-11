cmake_minimum_required(VERSION 3.10)
project(InvasionCycle)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Set dependencies directory
set(DEPS_DIR "${CMAKE_SOURCE_DIR}/dependencies")

# Set STB root directory
set(STB_ROOT "${DEPS_DIR}/stb")
set(STB_INCLUDE_DIRS "${STB_ROOT}/include")

# Set GLM root directory
set(GLM_ROOT "${DEPS_DIR}/glm/glm")
set(GLM_INCLUDE_DIRS "${GLM_ROOT}")

# GLEW
set(GLEW_ROOT "${DEPS_DIR}/glew/glew-2.2.0")
set(GLEW_INCLUDE_DIRS "${GLEW_ROOT}/include")
set(GLEW_LIBRARIES "${GLEW_ROOT}/lib/Release/x64/glew32.lib")
set(GLEW_DLL "${GLEW_ROOT}/bin/Release/x64/glew32.dll")

# GLFW
set(GLFW_ROOT "${DEPS_DIR}/glfw/glfw-3.4.bin.WIN64")
set(GLFW_INCLUDE_DIRS "${GLFW_ROOT}/include")
set(GLFW_LIBRARIES "${GLFW_ROOT}/lib-vc2022/glfw3.lib")
set(GLFW_DLL "${GLFW_ROOT}/lib-vc2022/glfw3.dll")

# OpenAL
set(OPENAL_ROOT "${DEPS_DIR}/openal-soft-1.22.2-bin")
set(OPENAL_INCLUDE_DIR "${OPENAL_ROOT}/include")
set(OPENAL_LIBRARY "${OPENAL_ROOT}/libs/Win64/OpenAL32.lib")
set(OPENAL_DLL "${OPENAL_ROOT}/bin/Win64/soft_oal.dll")

# Set include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/Source
    ${CMAKE_SOURCE_DIR}/Source/Engine
    ${CMAKE_SOURCE_DIR}/Source/InvasionCycle
    ${CMAKE_SOURCE_DIR}/Source/InvasionCycle/Core
    ${GLEW_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${OPENAL_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/dependencies/glm/glm
)

# Add source files
file(GLOB_RECURSE SOURCES 
    "Source/Engine/*.cpp"
    "Source/InvasionCycle/*.cpp"
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    ${GLEW_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${OPENAL_LIBRARY}
    opengl32
)

# Copy DLLs to output directory
if(WIN32)
    file(COPY ${GLEW_DLL} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    file(COPY ${GLFW_DLL} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    file(COPY ${OPENAL_DLL} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

# Copy assets to build directory
file(COPY ${CMAKE_SOURCE_DIR}/Assets DESTINATION ${CMAKE_BINARY_DIR})

# Set compiler flags
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif() 