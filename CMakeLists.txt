# Set cmake minimum version.
cmake_minimum_required(VERSION 3.16)

# Set the project details.
set(PROJECT_NAME ICT397-Lab2)
project(${PROJECT_NAME} LANGUAGES CXX)


# Disable in-source builds.
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)

# Set build type to debug by default.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Set build output directories.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG    ${PROJECT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG    ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE  ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG    ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE  ${PROJECT_BINARY_DIR}/lib)

# Define the executable.
add_executable(${PROJECT_NAME})

# Set C++ standard.
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Add source files.
add_subdirectory(src)
# Add third party libraries.
add_subdirectory(lib)

# Set compile flags.
target_compile_options(${PROJECT_NAME} PRIVATE
    # Clang
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Weverything>
    # GCC
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic -fdiagnostics-color=always>
    # Visual Studio
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

# Find dependencies.
find_package(OpenGL COMPONENTS OpenGL REQUIRED)

# Include and link against dependencies.
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL SDL2 SDL2main glm glad)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shader $<TARGET_FILE_DIR:${PROJECT_NAME}>/shader)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/images $<TARGET_FILE_DIR:${PROJECT_NAME}>/images)