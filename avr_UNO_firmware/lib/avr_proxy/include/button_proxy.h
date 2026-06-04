#ifndef BUTTON_PROXY_H
#define BUTTON_PROXY_H

#include <stdint.h>

typedef struct {
    uint8_t rotate_pin;
    uint8_t left_pin;
    uint8_t down_pin;
    uint8_t right_pin;
} button_proxy_t;

typedef struct {
    uint8_t rotate;
    uint8_t left;
    uint8_t down;
    uint8_t right;
} button_state_t;

void button_proxy_init(button_proxy_t *self);
button_state_t button_proxy_read(const button_proxy_t *self);

#endif