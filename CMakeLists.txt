cmake_minimum_required(VERSION 3.28)
project(HyperTicTacToe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default to Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# SFML (Dynamic) 
set(SFML_STATIC_LIBRARIES FALSE)

set(SFML_DIR "${CMAKE_SOURCE_DIR}/libraries/SFML-3.0.2/lib/cmake/SFML")

find_package(SFML 3.0.2 REQUIRED COMPONENTS Graphics)

# Sources
file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
set(SRC_INCLUDES_DIR "${CMAKE_SOURCE_DIR}/src/headers")

add_executable(app ${SRC_FILES})
target_include_directories(app PRIVATE ${SRC_INCLUDES_DIR})
target_link_libraries(app PRIVATE SFML::Graphics)

# Compile options 
target_compile_options(app PRIVATE
    $<$<CONFIG:Debug>:-g -O0>
    $<$<CONFIG:Release>:-O3>
)

# Preprocessor macros
target_compile_definitions(app PRIVATE
    $<$<CONFIG:Debug>:DEBUG_BUILD>
    $<$<CONFIG:Release>:RELEASE_BUILD>
)

# LTO (Release only)
set_property(TARGET app PROPERTY
    INTERPROCEDURAL_OPTIMIZATION
    $<$<CONFIG:Release>:TRUE>
)

# Copy dlls 
add_custom_command(TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:app>
        $<TARGET_FILE_DIR:app>
    COMMAND_EXPAND_LISTS
)

# Resources
file(COPY "${CMAKE_SOURCE_DIR}/res"
     DESTINATION "${CMAKE_BINARY_DIR}"
)
