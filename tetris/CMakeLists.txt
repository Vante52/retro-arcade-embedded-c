cmake_minimum_required(VERSION 3.10)
project(tetris C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(core)

add_executable(tetris
        main.c

        # Core
        core/bag.c
        core/board.c
        core/fsm.c
        core/game.c
        core/piece.c
        core/score.c

        # Host (terminal)
        host/attract.c
        host/display_terminal.c
        host/terminal_proxy.c
        host/input_keyboard.c
        host/keyboard_proxy_terminal.c
        host/main_host.c
)

target_compile_options(tetris PRIVATE -Wall -Wextra -Wpedantic)