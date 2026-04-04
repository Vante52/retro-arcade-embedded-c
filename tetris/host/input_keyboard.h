#ifndef TETRIS_INPUT_KEYBOARD_H
#define TETRIS_INPUT_KEYBOARD_H
#include "../platform/input_port.h"
#include "keyboard_proxy_terminal.h"

typedef struct {
    keyboard_proxy_terminal_t proxy;
}keyboard_input_t;

void input_keyboard_init(keyboard_input_t *keyboard);
input_cmd_t input_keyboard_poll(keyboard_input_t *keyboard);
void input_keyboard_shutdown(keyboard_input_t *keyboard);
input_port_t input_keyboard_create(keyboard_input_t *keyboard);
#endif //TETRIS_INPUT_KEYBOARD_H