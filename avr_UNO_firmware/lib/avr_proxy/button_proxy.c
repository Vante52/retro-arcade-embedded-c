#include "button_proxy.h"
#include <arduino_bridge.h>

void button_proxy_init(button_proxy_t *self) {
    bridge_pin_mode(self->rotate_pin, BRIDGE_INPUT);
    bridge_pin_mode(self->left_pin,   BRIDGE_INPUT);
    bridge_pin_mode(self->down_pin,   BRIDGE_INPUT);
    bridge_pin_mode(self->right_pin,  BRIDGE_INPUT);
}

static uint8_t read_pressed(uint8_t pin) {
    /* INPUT: LOW = no presionado */
    return (bridge_digital_read(pin) == BRIDGE_LOW) ? 0u : 1u;
}

button_state_t button_proxy_read(const button_proxy_t *self) {
    button_state_t state;

    state.rotate = read_pressed(self->rotate_pin);
    state.left   = read_pressed(self->left_pin);
    state.down   = read_pressed(self->down_pin);
    state.right  = read_pressed(self->right_pin);

    return state;
}