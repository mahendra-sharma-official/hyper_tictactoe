cmake_minimum_required(VERSION 3.28)
project(HyperTicTacToe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Release build
set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)

# Static SFML
set(SFML_STATIC_LIBRARIES TRUE)
add_compile_definitions(SFML_STATIC NDEBUG)

# Sources
file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
set(SRC_INCLUDES_DIR "${CMAKE_SOURCE_DIR}/src/headers")

# SFML config location
set(SFML_DIR "${CMAKE_SOURCE_DIR}/libraries/SFML-3.0.2/lib/cmake/SFML")

# SFML 3  Package
find_package(SFML 3.0.2 REQUIRED COMPONENTS Graphics)

add_executable(app ${SRC_FILES})
target_include_directories(app PRIVATE ${SRC_INCLUDES_DIR})

target_link_libraries(app PRIVATE SFML::Graphics)
target_link_options(app PRIVATE
    -static
    -static-libgcc
    -static-libstdc++
    -mwindows
    -s
)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

file(COPY "${CMAKE_SOURCE_DIR}/res"
DESTINATION "${CMAKE_BINARY_DIR}/")