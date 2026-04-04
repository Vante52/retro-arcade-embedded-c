#include "input_keyboard.h"
#include <stddef.h>

static void keyboard_port_init(void *ctx) {
    input_keyboard_init((keyboard_input_t *)ctx);
}

static input_cmd_t keyboard_port_poll(void *ctx) {
    return input_keyboard_poll((keyboard_input_t *)ctx);
}

static void keyboard_port_shutdown(void *ctx) {
    input_keyboard_shutdown((keyboard_input_t *)ctx);
}

void input_keyboard_init(keyboard_input_t *keyboard) {
    if (keyboard == NULL) return;
    keyboard_proxy_terminal_init(&keyboard->proxy);
}

input_cmd_t input_keyboard_poll(keyboard_input_t *keyboard) {
    if (keyboard == NULL) return INPUT_CMD_NONE;

    keyboard_key_t key = keyboard_proxy_terminal_read(&keyboard->proxy);

    switch (key) {
        case KEYBOARD_CMD_LEFT:
        case KEYBOARD_KEY_A:
            return INPUT_CMD_LEFT;

        case KEYBOARD_CMD_RIGHT:
        case KEYBOARD_KEY_D:
            return INPUT_CMD_RIGHT;

        case KEYBOARD_CMD_UP:
        case KEYBOARD_KEY_W:
        case KEYBOARD_KEY_SPACE:
            return INPUT_CMD_ROTATE;

        case KEYBOARD_CMD_DOWN:
        case KEYBOARD_KEY_S:
            return INPUT_CMD_DOWN;

        case KEYBOARD_KEY_Q:        /* salir */
            return INPUT_CMD_QUIT;

        case KEYBOARD_KEY_NONE:
        case KEYBOARD_KEY_UNKNOWN:
        default:
            return INPUT_CMD_NONE;
    }
}

void input_keyboard_shutdown(keyboard_input_t *keyboard) {
    if (keyboard == NULL) return;
    keyboard_proxy_terminal_shutdown(&keyboard->proxy);
}

input_port_t input_keyboard_create(keyboard_input_t *keyboard) {
    input_port_t port;
    port.init     = keyboard_port_init;
    port.poll     = keyboard_port_poll;
    port.shutdown = keyboard_port_shutdown;
    port.ctx      = keyboard;
    return port;
}