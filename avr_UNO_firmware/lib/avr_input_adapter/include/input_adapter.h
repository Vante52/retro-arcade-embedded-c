#ifndef INPUT_ADAPTER_H
#define INPUT_ADAPTER_H

#include <stdint.h>
#include <input_port.h>
#include <button_proxy.h>

typedef struct {
    button_proxy_t *buttons;

    button_state_t stable;
    button_state_t last_raw;

    uint32_t last_change_ms_left;
    uint32_t last_change_ms_down;
    uint32_t last_change_ms_right;
    uint32_t last_change_ms_rotate;

    uint32_t last_down_repeat_ms;

    uint8_t prev_left;
    uint8_t prev_down;
    uint8_t prev_right;
    uint8_t prev_rotate;

    uint16_t debounce_ms;
    uint16_t down_repeat_ms;
} input_adapter_t;

void input_adapter_init(input_adapter_t *self, button_proxy_t *buttons);
input_port_t input_adapter_create_port(input_adapter_t *self);

#endif