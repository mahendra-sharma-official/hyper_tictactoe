cmake_minimum_required(VERSION 3.28)
project(HyperTicTacToe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
cmake_policy(SET CMP0141 NEW)
set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

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
set(SRC_INCLUDES_DIR "${CMAKE_SOURCE_DIR}/src/")

add_executable(app ${SRC_FILES})
target_include_directories(app PRIVATE ${SRC_INCLUDES_DIR})
target_link_libraries(app PRIVATE SFML::Graphics)

# All configs go into a 'game' folder inside the build directory
set(GAME_OUTPUT_DIR "${CMAKE_BINARY_DIR}/game")

# Set executable output folder
set_target_properties(app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${GAME_OUTPUT_DIR}"
    PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"                  # debug info
    IMPORT_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"       # .lib / .dll import
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${GAME_OUTPUT_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${GAME_OUTPUT_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${GAME_OUTPUT_DIR}"
)


if(MSVC)
    # Compile options
    target_compile_options(app PRIVATE
        $<$<CONFIG:Debug>:/Od /Zi>        # Disable optimization + debug info
        $<$<CONFIG:Release>:/O2 /DNDEBUG> # Full optimization + NDEBUG
    )

    # Link options
    target_link_options(app PRIVATE
        $<$<CONFIG:Debug>:/DEBUG>            # Enable debug info
        $<$<CONFIG:Release>:/SUBSYSTEM:windows>
        $<$<CONFIG:Release>:/ENTRY:mainCRTStartup>
    )
endif()


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

# Copy dlls (SFML)
set(SFML_DLL_DIR "${SFML_DIR}/../../../bin")
add_custom_command(TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:app>/dlls
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<$<CONFIG:Debug>:${SFML_DLL_DIR}/sfml-graphics-d-3.dll>
        $<$<CONFIG:Debug>:${SFML_DLL_DIR}/sfml-window-d-3.dll>
        $<$<CONFIG:Debug>:${SFML_DLL_DIR}/sfml-system-d-3.dll>
        $<$<CONFIG:Release>:${SFML_DLL_DIR}/sfml-graphics-3.dll>
        $<$<CONFIG:Release>:${SFML_DLL_DIR}/sfml-window-3.dll>
        $<$<CONFIG:Release>:${SFML_DLL_DIR}/sfml-system-3.dll>
        "${GAME_OUTPUT_DIR}"
    COMMAND_EXPAND_LISTS
)


# Copy resources 
add_custom_command(TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/res"
        "${GAME_OUTPUT_DIR}/res"
)
