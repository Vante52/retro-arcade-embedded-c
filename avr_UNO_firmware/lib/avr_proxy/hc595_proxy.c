#include "hc595_proxy.h"
#include <arduino_bridge.h>

void hc595_proxy_init(hc595_proxy_t *self) {
    bridge_pin_mode(self->data_pin, BRIDGE_OUTPUT);
    bridge_pin_mode(self->latch_pin, BRIDGE_OUTPUT);
    bridge_pin_mode(self->clock_pin, BRIDGE_OUTPUT);
    bridge_pin_mode(self->oe_pin, BRIDGE_OUTPUT);
    bridge_pin_mode(self->reset_pin, BRIDGE_OUTPUT);

    /* OE activo en LOW, MR activo en LOW */
    bridge_digital_write(self->oe_pin, BRIDGE_LOW);
    bridge_digital_write(self->reset_pin, BRIDGE_HIGH);
    bridge_digital_write(self->latch_pin, BRIDGE_HIGH);
}

void hc595_proxy_enable(hc595_proxy_t *self) {
    bridge_digital_write(self->oe_pin, BRIDGE_LOW);
}

void hc595_proxy_disable(hc595_proxy_t *self) {
    bridge_digital_write(self->oe_pin, BRIDGE_HIGH);
}

void hc595_proxy_reset(hc595_proxy_t *self) {
    bridge_digital_write(self->reset_pin, BRIDGE_LOW);
    bridge_delay_us(2);
    bridge_digital_write(self->reset_pin, BRIDGE_HIGH);
}

void hc595_proxy_write(hc595_proxy_t *self,
                              uint8_t b3,
                              uint8_t b2,
                              uint8_t b1,
                              uint8_t b0) {
    bridge_digital_write(self->latch_pin, BRIDGE_LOW);

    bridge_shift_out(self->data_pin, self->clock_pin, BRIDGE_MSBFIRST, b3);
    bridge_shift_out(self->data_pin, self->clock_pin, BRIDGE_MSBFIRST, b2);
    bridge_shift_out(self->data_pin, self->clock_pin, BRIDGE_MSBFIRST, b1);
    bridge_shift_out(self->data_pin, self->clock_pin, BRIDGE_MSBFIRST, b0);

    bridge_digital_write(self->latch_pin, BRIDGE_HIGH);
}