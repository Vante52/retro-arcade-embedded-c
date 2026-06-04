#ifndef ARDUINO_BRIDGE_H
#define ARDUINO_BRIDGE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
#define BRIDGE_LOW  0u
#define BRIDGE_HIGH 1u

#define BRIDGE_INPUT         0u
#define BRIDGE_OUTPUT        1u
#define BRIDGE_INPUT_PULLUP  2u

#define BRIDGE_LSBFIRST 0u
#define BRIDGE_MSBFIRST 1u

void bridge_pin_mode(uint8_t pin, uint8_t mode);

void bridge_digital_write(uint8_t pin, uint8_t value);
uint8_t bridge_digital_read(uint8_t pin);

void bridge_shift_out(uint8_t data_pin,
                      uint8_t clock_pin,
                      uint8_t bit_order,
                      uint8_t value);

void bridge_delay_us(uint16_t us);
void bridge_delay_ms(uint16_t ms);

uint32_t bridge_millis(void);
uint32_t bridge_micros(void);

void bridge_serial_begin(uint32_t baud);
void bridge_serial_print(const char *msg);
void bridge_serial_println(const char *msg);
void bridge_serial_print_u8(uint8_t value);
void bridge_serial_println_u8(uint8_t value);

#ifdef __cplusplus
}
#endif
#endif