#ifndef TETRIS_KEYBOARD_PROXY_TERMINAL_H
#define TETRIS_KEYBOARD_PROXY_TERMINAL_H

#include <stdint.h>
#include <termios.h>

typedef enum {
    KEYBOARD_CMD_NONE,
    KEYBOARD_CMD_ROTATE,
    KEYBOARD_CMD_LEFT,
    KEYBOARD_CMD_RIGHT,
    KEYBOARD_CMD_DOWN,
    KEYBOARD_CMD_UNKNOWN,
    KEYBOARD_CMD_UP,
    KEYBOARD_KEY_A,
    KEYBOARD_KEY_D,
    KEYBOARD_KEY_W,
    KEYBOARD_KEY_S,
    KEYBOARD_KEY_Q,
    KEYBOARD_KEY_SPACE,
    KEYBOARD_KEY_UNKNOWN,
    KEYBOARD_KEY_NONE,
}keyboard_key_t;

typedef struct {
    struct termios tty;
    int flags;
    uint8_t initialized;
}keyboard_proxy_terminal_t;

void keyboard_proxy_terminal_init(keyboard_proxy_terminal_t *proxy);
keyboard_key_t keyboard_proxy_terminal_read(keyboard_proxy_terminal_t *proxy);
void keyboard_proxy_terminal_shutdown(keyboard_proxy_terminal_t *proxy);

#endif //TETRIS_KEYBOARD_PROXY_TERMINAL_H