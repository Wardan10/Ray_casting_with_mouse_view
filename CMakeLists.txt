cmake_minimum_required(VERSION 3.10)
project(GameProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output executable to bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Source files
file(GLOB SOURCES
    ${CMAKE_SOURCE_DIR}/game_src/*.cpp
)

# ======================
# SFML paths (Windows)
# ======================
if(WIN32)
    # Change these to your SFML installation paths
    set(SFML_ROOT "C:/SFML")
    set(SFML_INCLUDE_DIR "${SFML_ROOT}/include")
    set(SFML_LIB_DIR "${SFML_ROOT}/lib")
    set(SFML_DLL_DIR "${SFML_ROOT}/bin")

    include_directories(${SFML_INCLUDE_DIR})
    link_directories(${SFML_LIB_DIR})

    add_executable(game ${SOURCES})

    target_link_libraries(game
        sfml-graphics
        sfml-window
        sfml-system
    )

    # Copy DLLs after build
    add_custom_command(TARGET game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SFML_DLL_DIR}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    )
else()
    # Linux or macOS — assumes SFML installed system-wide
    include_directories(    
        ${CMAKE_SOURCE_DIR}/game_src/include
        ${CMAKE_SOURCE_DIR}/game_src/include/Game
        ${CMAKE_SOURCE_DIR}/game_src/include/networking
    )
    add_executable(game ${SOURCES})
    target_link_libraries(game sfml-graphics sfml-window sfml-system X11)
endif()
