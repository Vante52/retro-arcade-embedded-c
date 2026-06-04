#include <Arduino.h>
#include <arduino_bridge.h>

extern "C" {

void bridge_pin_mode(uint8_t pin, uint8_t mode) {
    switch (mode) {
        case BRIDGE_OUTPUT:
            pinMode(pin, OUTPUT);
            break;
        case BRIDGE_INPUT_PULLUP:
            pinMode(pin, INPUT_PULLUP);
            break;
        case BRIDGE_INPUT:
        default:
            pinMode(pin, INPUT);
            break;
    }
}

void bridge_digital_write(uint8_t pin, uint8_t value) {
    digitalWrite(pin, value ? HIGH : LOW);
}

uint8_t bridge_digital_read(uint8_t pin) {
    return (digitalRead(pin) == HIGH) ? BRIDGE_HIGH : BRIDGE_LOW;
}

void bridge_shift_out(uint8_t data_pin,
                      uint8_t clock_pin,
                      uint8_t bit_order,
                      uint8_t value) {
    shiftOut(data_pin,
             clock_pin,
             (bit_order == BRIDGE_LSBFIRST) ? LSBFIRST : MSBFIRST,
             value);
}

void bridge_delay_us(uint16_t us) {
    delayMicroseconds(us);
}

void bridge_delay_ms(uint16_t ms) {
    delay(ms);
}

uint32_t bridge_millis(void) {
    return millis();
}

uint32_t bridge_micros(void) {
    return micros();
}

void bridge_serial_begin(uint32_t baud) {
    Serial.begin(baud);
}

void bridge_serial_print(const char *msg) {
    if (msg != nullptr) {
        Serial.print(msg);
    }
}

void bridge_serial_println(const char *msg) {
    if (msg != nullptr) {
        Serial.println(msg);
    } else {
        Serial.println();
    }
}

void bridge_serial_print_u8(uint8_t value) {
    Serial.print(value);
}

void bridge_serial_println_u8(uint8_t value) {
    Serial.println(value);
}

} /* extern "C" */