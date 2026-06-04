#ifndef HC595_PROXY_H
#define HC595_PROXY_H

#include <stdint.h>

typedef struct {
    uint8_t oe_pin;
    uint8_t data_pin;
    uint8_t clock_pin;
    uint8_t latch_pin;
    uint8_t reset_pin;
} hc595_proxy_t;

void hc595_proxy_init(hc595_proxy_t *self);
void hc595_proxy_enable(hc595_proxy_t *self);
void hc595_proxy_disable(hc595_proxy_t *self);
void hc595_proxy_reset(hc595_proxy_t *self);
void hc595_proxy_write(hc595_proxy_t *self, uint8_t b3, uint8_t b2, uint8_t b1, uint8_t b0);

#endif