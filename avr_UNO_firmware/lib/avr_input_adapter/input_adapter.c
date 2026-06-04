#include <input_adapter.h>
#include <arduino_bridge.h>

static void input_adapter_port_init(void *ctx) {
    (void)ctx;
}

static void input_adapter_port_shutdown(void *ctx) {
    (void)ctx;
}

static void update_one_button(uint8_t raw,
                              uint8_t *stable_value,
                              uint8_t *prev_value,
                              uint32_t *last_change_ms,
                              uint16_t debounce_ms,
                              uint32_t now_ms) {
    if (raw != *stable_value) {
        if ((uint32_t)(now_ms - *last_change_ms) >= debounce_ms) {
            *prev_value = *stable_value;
            *stable_value = raw;
            *last_change_ms = now_ms;
        }
    } else {
        *last_change_ms = now_ms;
    }
}

static input_cmd_t input_adapter_port_poll(void *ctx) {
    input_adapter_t *self = (input_adapter_t *)ctx;
    button_state_t raw = button_proxy_read(self->buttons);
    uint32_t now_ms = bridge_millis();

    update_one_button(raw.left,
                      &self->stable.left,
                      &self->prev_left,
                      &self->last_change_ms_left,
                      self->debounce_ms,
                      now_ms);

    update_one_button(raw.down,
                      &self->stable.down,
                      &self->prev_down,
                      &self->last_change_ms_down,
                      self->debounce_ms,
                      now_ms);

    update_one_button(raw.right,
                      &self->stable.right,
                      &self->prev_right,
                      &self->last_change_ms_right,
                      self->debounce_ms,
                      now_ms);

    update_one_button(raw.rotate,
                      &self->stable.rotate,
                      &self->prev_rotate,
                      &self->last_change_ms_rotate,
                      self->debounce_ms,
                      now_ms);

    /* flanco de subida lógico: 0 -> 1 */
    if (self->stable.rotate && !self->prev_rotate) {
        self->prev_rotate = self->stable.rotate;
        return INPUT_CMD_ROTATE;
    }

    if (self->stable.left && !self->prev_left) {
        self->prev_left = self->stable.left;
        return INPUT_CMD_LEFT;
    }

    if (self->stable.right && !self->prev_right) {
        self->prev_right = self->stable.right;
        return INPUT_CMD_RIGHT;
    }

    /* DOWN: permite repetición mientras se mantiene pulsado */
    if (self->stable.down) {
        if (!self->prev_down ||
            (uint32_t)(now_ms - self->last_down_repeat_ms) >= self->down_repeat_ms) {
            self->prev_down = self->stable.down;
            self->last_down_repeat_ms = now_ms;
            return INPUT_CMD_DOWN;
        }
    } else {
        self->prev_down = 0u;
    }

    /* limpia prev_* cuando se sueltan */
    if (!self->stable.left)   self->prev_left = 0u;
    if (!self->stable.right)  self->prev_right = 0u;
    if (!self->stable.rotate) self->prev_rotate = 0u;

    return INPUT_CMD_NONE;
}

void input_adapter_init(input_adapter_t *self, button_proxy_t *buttons) {
    self->buttons = buttons;

    self->stable.left = 0u;
    self->stable.down = 0u;
    self->stable.right = 0u;
    self->stable.rotate = 0u;

    self->last_raw = self->stable;

    self->prev_left = 0u;
    self->prev_down = 0u;
    self->prev_right = 0u;
    self->prev_rotate = 0u;

    self->last_change_ms_left = 0u;
    self->last_change_ms_down = 0u;
    self->last_change_ms_right = 0u;
    self->last_change_ms_rotate = 0u;

    self->last_down_repeat_ms = 0u;

    self->debounce_ms = 20u;
    self->down_repeat_ms = 80u;
}

input_port_t input_adapter_create_port(input_adapter_t *self) {
    input_port_t port;

    port.init = input_adapter_port_init;
    port.poll = input_adapter_port_poll;
    port.shutdown = input_adapter_port_shutdown;
    port.ctx = self;

    return port;
}