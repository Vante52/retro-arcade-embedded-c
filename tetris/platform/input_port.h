#ifndef INPUT_PORT_H
#define INPUT_PORT_H

typedef enum {
    INPUT_CMD_RIGHT,
    INPUT_CMD_LEFT,
    INPUT_CMD_DOWN,
    INPUT_CMD_ROTATE,
    INPUT_CMD_QUIT,
    INPUT_CMD_NONE
} input_cmd_t;

typedef struct {
    void (*init)(void *ctx);
    input_cmd_t (*poll)(void *ctx);
    void (*shutdown)(void *ctx);
    void *ctx;
} input_port_t;
#endif