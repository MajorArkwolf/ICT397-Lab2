# Library build options.
option(ASSIMP_BUILD_TESTS OFF)
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(OPTION_BUILD_TESTS OFF)

# Build libraries.
string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# Disable compiler warnings.
add_compile_options(
    # Clang
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Wno-everything>
    # GCC
    $<$<CXX_COMPILER_ID:GNU>:-w>
    # Visual Studio
    $<$<CXX_COMPILER_ID:MSVC>:/W0>
)


add_subdirectory(glm)
add_subdirectory(glad)
set(SDL_STATIC OFF)
set(SDL_DYNAMIC ON)
set(SDL_SHARED ON)
set(BUILD_SHARED_LIBS ON)
add_subdirectory(sdl2)

# Ignore warnings from library headers.
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    sdl2/include
    sdl2/src
    glad/include
)