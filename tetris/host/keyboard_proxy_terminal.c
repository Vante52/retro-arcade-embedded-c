#include "keyboard_proxy_terminal.h"

#include <ctype.h>
#include <fcntl.h>
#include <stddef.h>
#include <unistd.h>

static int read_byte_nonblocking(unsigned char *out_byte) {
    if (out_byte == NULL) return 0;
    ssize_t n = read(STDIN_FILENO, out_byte, 1);
    return (n == 1) ? 1 : 0;
}

void keyboard_proxy_terminal_init(keyboard_proxy_terminal_t *proxy) {
    struct termios raw;
    int flags;

    if (proxy == NULL || proxy->initialized) return;

    if (tcgetattr(STDIN_FILENO, &proxy->tty) != 0) return;

    raw = proxy->tty;
    raw.c_lflag &= (tcflag_t)~(ICANON | ECHO);
    raw.c_cc[VMIN]  = 0;
    raw.c_cc[VTIME] = 0;

    if (tcsetattr(STDIN_FILENO, TCSANOW, &raw) != 0) return;

    flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    if (flags < 0) {
        (void)tcsetattr(STDIN_FILENO, TCSANOW, &proxy->tty);
        return;
    }

    proxy->flags = flags;

    if (fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK) != 0) {
        (void)tcsetattr(STDIN_FILENO, TCSANOW, &proxy->tty);
        return;
    }

    proxy->initialized = 1u;
}

keyboard_key_t keyboard_proxy_terminal_read(keyboard_proxy_terminal_t *proxy) {
    unsigned char ch, next1, next2;

    (void)proxy;

    if (!read_byte_nonblocking(&ch)) return KEYBOARD_KEY_NONE;

    if (ch == 27u) {                            /* secuencia de escape */
        if (!read_byte_nonblocking(&next1)) return KEYBOARD_KEY_UNKNOWN;
        if (next1 != '[')                       return KEYBOARD_KEY_UNKNOWN;
        if (!read_byte_nonblocking(&next2)) return KEYBOARD_KEY_UNKNOWN;

        switch (next2) {
            case 'A': return KEYBOARD_CMD_UP;
            case 'B': return KEYBOARD_CMD_DOWN;
            case 'C': return KEYBOARD_CMD_RIGHT;
            case 'D': return KEYBOARD_CMD_LEFT;
            default:  return KEYBOARD_KEY_UNKNOWN;
        }
    }

    ch = (unsigned char)tolower((int)ch);

    switch (ch) {
        case 'a': return KEYBOARD_KEY_A;
        case 'd': return KEYBOARD_KEY_D;
        case 'w': return KEYBOARD_KEY_W;
        case 's': return KEYBOARD_KEY_S;
        case 'q': return KEYBOARD_KEY_Q;
        case ' ': return KEYBOARD_KEY_SPACE;
        default:  return KEYBOARD_KEY_UNKNOWN;
    }
}

void keyboard_proxy_terminal_shutdown(keyboard_proxy_terminal_t *proxy) {
    if (proxy == NULL || !proxy->initialized) return;

    (void)tcsetattr(STDIN_FILENO, TCSANOW, &proxy->tty);
    (void)fcntl(STDIN_FILENO, F_SETFL, proxy->flags);

    proxy->initialized = 0u;
}