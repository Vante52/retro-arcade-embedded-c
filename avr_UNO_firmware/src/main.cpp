#include <Arduino.h>

extern "C" {
#include <firmware.h>
}

void setup(void) {
    firmware_init();
}

void loop(void) {
    firmware_run_once();
}