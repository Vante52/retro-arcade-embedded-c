cmake_minimum_required(VERSION 3.10)
project(tetris C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ================================================================
# Núcleo del juego como librería estática
# ================================================================
add_library(tetris_core STATIC
    core/bag.c
    core/board.c
    core/fsm.c
    core/game.c
    core/piece.c
    core/score.c
)

target_include_directories(tetris_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/core
)

target_compile_options(tetris_core PRIVATE -Wall -Wextra -Wpedantic)

# ================================================================
# Ejecutable de terminal
# ================================================================
add_executable(tetris_terminal
    main_terminal.c
    host/terminal/attract.c
    host/input_keyboard.c
    host/main_host.c
    host/terminal/display_terminal.c
    host/terminal/terminal_proxy.c
    host/terminal/keyboard_proxy_terminal.c
)

target_include_directories(tetris_terminal
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/core
        ${CMAKE_CURRENT_SOURCE_DIR}/host
        ${CMAKE_CURRENT_SOURCE_DIR}/host/terminal
        ${CMAKE_CURRENT_SOURCE_DIR}/platform
)

target_compile_options(tetris_terminal PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(tetris_terminal PRIVATE tetris_core)

add_custom_target(run_terminal
    COMMAND tetris_terminal
    DEPENDS tetris_terminal
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    USES_TERMINAL
)

# ================================================================
# Ejecutable SDL2
# ================================================================
find_package(SDL2 REQUIRED)

add_executable(tetris_sdl
    main_sdl.c
    host/sdl/sdl_proxy.c
    host/sdl/display_sdl.c
    host/sdl/input_sdl.c
    host/sdl/main_sdl.c
)

target_include_directories(tetris_sdl
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/core
        ${CMAKE_CURRENT_SOURCE_DIR}/host
        ${CMAKE_CURRENT_SOURCE_DIR}/host/sdl
        ${CMAKE_CURRENT_SOURCE_DIR}/platform
        ${SDL2_INCLUDE_DIRS}
)

target_compile_options(tetris_sdl PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(tetris_sdl PRIVATE tetris_core ${SDL2_LIBRARIES})

add_custom_target(run_sdl
    COMMAND tetris_sdl
    DEPENDS tetris_sdl
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    USES_TERMINAL
)