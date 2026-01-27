cmake_minimum_required(VERSION 3.20)
project(HyperTicTacToe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# SFML3
set(SFML_DIR "${CMAKE_SOURCE_DIR}/libraries/SFML-3.0.2")
set(SFML_INCLUDE_DIR "${SFML_DIR}/include")
set(SFML_LIB_DIR "${SFML_DIR}/lib")

include_directories(${SFML_INCLUDE_DIR}) 
# Executable
add_executable(app "${SRC_FILES}")


# Link SFML libraries manually
link_directories(${SFML_LIB_DIR})
target_link_libraries(app
    ${SFML_LIB_DIR}/libsfml-graphics.a
    ${SFML_LIB_DIR}/libsfml-window.a
    ${SFML_LIB_DIR}/libsfml-system.a
)

# Path to the SFML DLLs
set(SFML_BIN_DIR "${CMAKE_SOURCE_DIR}/libraries/SFML-3.0.2/bin/")

# List of DLLs to copy
set(SFML_DLLS
    sfml-graphics-3.dll
    sfml-window-3.dll
    sfml-system-3.dll
)

# Copy DLLs to the executable folder after build
foreach(dll ${SFML_DLLS})
    add_custom_command(TARGET app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SFML_BIN_DIR}/${dll}"
        $<TARGET_FILE_DIR:app>
    )
endforeach()

