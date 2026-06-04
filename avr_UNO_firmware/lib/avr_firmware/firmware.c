#include <stdint.h>

#include "firmware.h"
#include <arduino_bridge.h>
#include "hc595_proxy.h"
#include "button_proxy.h"
#include "dual_matrix_proxy.h"
#include <display_port.h>
#include <game.h>
#include <board.h>
#include <display_adapter.h>
#include <fsm.h>
#include <game.h>
#include <input_adapter.h>
#include <input_port.h>

static dual_matrix_proxy_t g_display;
static display_avr_t g_display_adapter;
static display_port_t g_display_port;
static input_adapter_t g_input_adapter;
static input_port_t g_input_port;

static game_t *g_game = 0;
static fsm_t g_fsm;

/* Pines de tu PCB */
enum {
    PIN_OE     = 2,
    PIN_LATCH  = 3,
    PIN_ROTATE = 12,
    PIN_CLOCK  = 7,
    PIN_RESET  = 8,
    PIN_DATA   = 9,
    PIN_LEFT   = 11,
    PIN_DOWN   = 10,
    PIN_RIGHT  = 4
};

static hc595_proxy_t g_chain = {
    .oe_pin    = PIN_OE,
    .latch_pin = PIN_LATCH,
    .clock_pin = PIN_CLOCK,
    .reset_pin = PIN_RESET,
    .data_pin  = PIN_DATA
};

static button_proxy_t g_buttons = {
    .rotate_pin = PIN_ROTATE,
    .left_pin   = PIN_LEFT,
    .down_pin   = PIN_DOWN,
    .right_pin  = PIN_RIGHT
};

static void render_game_to_display(const game_t *game, display_port_t *display);

static dual_matrix_proxy_t g_display;

static void render_game_to_display(const game_t *game, display_port_t *display) {
    uint8_t x, y, row, col;

    display->begin_frame(display->ctx);

    for (y = 0u; y < BOARD_HEIGHT; y++) {
        for (x = 0u; x < BOARD_WIDTH; x++) {
            uint8_t value = (game_board_cell(game, x, y) != 0u) ? 1u : 0u;
            display->draw_cell(display->ctx, x, y, value);
        }
    }

    for (row = 0u; row < 4u; row++) {
        for (col = 0u; col < 4u; col++) {
            if (game_current_piece_cell(game, row, col)) {
                int8_t px = game_current_piece_x(game) + (int8_t)col;
                int8_t py = game_current_piece_y(game) + (int8_t)row;

                if (px >= 0 && px < BOARD_WIDTH &&
                    py >= 0 && py < BOARD_HEIGHT) {
                    display->draw_cell(display->ctx, (uint8_t)px, (uint8_t)py, 1u);
                }
            }
        }
    }

    display->end_frame(display->ctx);
}

void firmware_init(void) {
    bridge_serial_begin(9600);

    hc595_proxy_init(&g_chain);
    button_proxy_init(&g_buttons);

    input_adapter_init(&g_input_adapter, &g_buttons);
    g_input_port = input_adapter_create_port(&g_input_adapter);
    g_input_port.init(g_input_port.ctx);

    dual_matrix_proxy_init(&g_display, &g_chain);

    display_avr_init(&g_display_adapter, &g_display);
    g_display_port = display_avr_create_port(&g_display_adapter);

    g_game = game_get_instance();
    game_init(g_game);
    fsm_init(&g_fsm);

    /* primer tick para spawn */
    fsm_dispatch(&g_fsm, g_game, FSM_EVENT_TICK);
}

static void process_input(void) {
    input_cmd_t cmd = g_input_port.poll(g_input_port.ctx);

    switch (cmd) {
        case INPUT_CMD_LEFT:
            fsm_dispatch(&g_fsm, g_game, FSM_EVENT_MOVE_LEFT);
            break;

        case INPUT_CMD_RIGHT:
            fsm_dispatch(&g_fsm, g_game, FSM_EVENT_MOVE_RIGHT);
            break;

        case INPUT_CMD_ROTATE:
            fsm_dispatch(&g_fsm, g_game, FSM_EVENT_ROTATE);
            break;

        case INPUT_CMD_DOWN:
            fsm_dispatch(&g_fsm, g_game, FSM_EVENT_DOWN);
            break;

        case INPUT_CMD_NONE:
        default:
            break;
    }
}
void firmware_run_once(void) {
    static uint32_t last_render_us = 0u;
    static uint32_t last_tick_ms = 0u;

    uint32_t now_us = bridge_micros();
    uint32_t now_ms = bridge_millis();

    dual_matrix_proxy_scan_step(&g_display);

    process_input();

    if ((uint32_t)(now_ms - last_tick_ms) >= 300u) {
        fsm_dispatch(&g_fsm, g_game, FSM_EVENT_TICK);
        last_tick_ms = now_ms;
    }

    if ((uint32_t)(now_us - last_render_us) >= 5000u) {
        render_game_to_display(g_game, &g_display_port);
        last_render_us = now_us;
    }

    bridge_delay_us(500);
}