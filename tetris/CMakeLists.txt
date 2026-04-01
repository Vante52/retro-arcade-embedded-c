cmake_minimum_required(VERSION 4.1)
project(tetris C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(core)

add_executable(tetris
        core/bag.c
        core/bag.h
        core/board.c
        core/board.h
        core/fsm.c
        core/fsm.h
        core/game.c
        core/game.h
        core/piece.c
        core/piece.h
        core/score.c
        core/score.h
        main.c
        platform/display_port.h
        host/display_terminal.c
        host/terminal_proxy.h
)
