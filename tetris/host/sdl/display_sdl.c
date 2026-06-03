#include "display_sdl.h"
#include "../../core/piece.h"
#include "../../core/board.h"
#include <SDL2/SDL.h>
#include <stdint.h>
#include <string.h>

/* ================================================================
 * PALETA DE COLORES
 * ================================================================ */

/* Fondo de tablero y ventana */
#define COL_BG       { 10,  10,  20, 255}
#define COL_CELL_BG  { 16,  16,  32, 255}
#define COL_GRID     { 28,  28,  52, 255}
#define COL_BORDER   { 70,  70, 180, 255}
#define COL_PANEL_BG { 14,  14,  28, 255}
#define COL_WHITE    {230, 230, 255, 255}
#define COL_YELLOW   {240, 220,   0, 255}
#define COL_DIMWHITE { 90,  90, 120, 255}

/* Colores por tipo de pieza (índice = piece_type + 1)
 *  0 = vacío (no se usa en draw_cell)
 *  1 = I  2 = O  3 = T  4 = J  5 = L  6 = S  7 = Z   */
static const SDL_Color PIECE_COLOR[8] = {
    {  0,   0,   0, 255},   /* 0 – vacío           */
    {  0, 220, 220, 255},   /* 1 – I  cian         */
    {220, 220,   0, 255},   /* 2 – O  amarillo     */
    {160,   0, 200, 255},   /* 3 – T  púrpura      */
    { 30,  80, 255, 255},   /* 4 – J  azul         */
    {230, 130,   0, 255},   /* 5 – L  naranja      */
    {  0, 200,  60, 255},   /* 6 – S  verde        */
    {220,  30,  30, 255},   /* 7 – Z  rojo         */
};

/* ================================================================
 * FUENTE DE PÍXELES 4×6  (0–9, A–Z subset)
 * Cada fila es un uint8_t; bit 3 = columna 0 (izquierda).
 * ================================================================ */
#define FONT_CHAR_W  4
#define FONT_SCALE   2   /* px por píxel de fuente → 8×12 por char */
#define FONT_GAP     2   /* px entre caracteres */

/* Índices: '0'-'0'=0 … '9'-'0'=9, luego A-Z a partir de 10 */
static const uint8_t PIXEL_FONT[][FONT_CHAR_H] = {
/* 0 */ {0x6,0x9,0x9,0x9,0x9,0x6},
/* 1 */ {0x2,0x6,0x2,0x2,0x2,0x7},
/* 2 */ {0x6,0x9,0x1,0x2,0x4,0xF},
/* 3 */ {0xE,0x1,0x6,0x1,0x1,0xE},
/* 4 */ {0x9,0x9,0xF,0x1,0x1,0x1},
/* 5 */ {0xF,0x8,0xE,0x1,0x1,0xE},
/* 6 */ {0x6,0x8,0xE,0x9,0x9,0x6},
/* 7 */ {0xF,0x1,0x2,0x2,0x4,0x4},
/* 8 */ {0x6,0x9,0x6,0x9,0x9,0x6},
/* 9 */ {0x6,0x9,0x7,0x1,0x1,0x6},
/* A */ {0x6,0x9,0xF,0x9,0x9,0x9},
/* B */ {0xE,0x9,0xE,0x9,0x9,0xE},
/* C */ {0x6,0x9,0x8,0x8,0x9,0x6},
/* D */ {0xE,0x9,0x9,0x9,0x9,0xE},
/* E */ {0xF,0x8,0xE,0x8,0x8,0xF},
/* F */ {0xF,0x8,0xE,0x8,0x8,0x8},
/* G */ {0x6,0x9,0x8,0xB,0x9,0x6},
/* H */ {0x9,0x9,0xF,0x9,0x9,0x9},
/* I */ {0xE,0x4,0x4,0x4,0x4,0xE},
/* J */ {0x1,0x1,0x1,0x1,0x9,0x6},
/* K */ {0x9,0xA,0xC,0xA,0x9,0x9},
/* L */ {0x8,0x8,0x8,0x8,0x8,0xF},
/* M */ {0x9,0xF,0xF,0x9,0x9,0x9},
/* N */ {0x9,0xD,0xB,0x9,0x9,0x9},
/* O */ {0x6,0x9,0x9,0x9,0x9,0x6},
/* P */ {0xE,0x9,0x9,0xE,0x8,0x8},
/* Q */ {0x6,0x9,0x9,0xB,0x9,0x7},
/* R */ {0xE,0x9,0x9,0xE,0xA,0x9},
/* S */ {0x6,0x9,0x4,0x2,0x9,0x6},
/* T */ {0xE,0x4,0x4,0x4,0x4,0x4},
/* U */ {0x9,0x9,0x9,0x9,0x9,0x6},
/* V */ {0x9,0x9,0x9,0x9,0x6,0x4},
/* W */ {0x9,0x9,0x9,0xF,0xF,0x9},
/* X */ {0x9,0x9,0x6,0x6,0x9,0x9},
/* Y */ {0x9,0x9,0x6,0x4,0x4,0x4},
/* Z */ {0xF,0x1,0x2,0x4,0x8,0xF},
};

static int char_to_idx(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'Z') return 10 + (c - 'A');
    if (c >= 'a' && c <= 'z') return 10 + (c - 'a');
    return -1;
}

/* Dibuja un string con la fuente de píxeles en la posición (px, py) */
static void draw_text(SDL_Renderer *r, const char *s,
                      int px, int py, SDL_Color col) {
    SDL_SetRenderDrawColor(r, col.r, col.g, col.b, col.a);
    int cx = px;
    for (int i = 0; s[i]; i++) {
        int idx = char_to_idx(s[i]);
        if (idx >= 0) {
            for (int row = 0; row < FONT_CHAR_H; row++) {
                uint8_t bits = PIXEL_FONT[idx][row];
                for (int col_bit = 0; col_bit < FONT_CHAR_W; col_bit++) {
                    if (bits & (1u << (3 - col_bit))) {
                        SDL_Rect dot = {
                            cx + col_bit * FONT_SCALE,
                            py + row    * FONT_SCALE,
                            FONT_SCALE, FONT_SCALE
                        };
                        SDL_RenderFillRect(r, &dot);
                    }
                }
            }
        }
        cx += FONT_CHAR_W * FONT_SCALE + FONT_GAP;
    }
}

/* Convierte un uint32_t a string decimal (sin stdlib completo) */
static void u32_to_str(uint32_t v, char *buf, int buflen) {
    if (buflen < 2) return;
    if (v == 0) { buf[0] = '0'; buf[1] = '\0'; return; }
    char tmp[12];
    int  i = 0;
    while (v > 0 && i < 11) { tmp[i++] = (char)('0' + v % 10); v /= 10; }
    int j = 0;
    while (i > 0 && j < buflen - 1) buf[j++] = tmp[--i];
    buf[j] = '\0';
}

/* ================================================================
 * DIBUJO DE UNA CELDA CON BISEL
 * color_idx: 0=vacío  1-7=pieza (mapeado a PIECE_COLOR)
 * ================================================================ */
static void draw_cell(SDL_Renderer *r, int px, int py, int sz, uint8_t color_idx) {
    if (color_idx == 0) {
        /* Celda vacía: fondo oscuro + línea de cuadrícula */
        SDL_Color bg = (SDL_Color)COL_CELL_BG;
        SDL_SetRenderDrawColor(r, bg.r, bg.g, bg.b, 255);
        SDL_Rect rect = {px, py, sz, sz};
        SDL_RenderFillRect(r, &rect);

        SDL_Color gr = (SDL_Color)COL_GRID;
        SDL_SetRenderDrawColor(r, gr.r, gr.g, gr.b, 255);
        SDL_RenderDrawRect(r, &rect);
        return;
    }

    SDL_Color c = PIECE_COLOR[color_idx];

    /* Relleno principal (con 2px de margen interior para el bisel) */
    SDL_Rect inner = {px + 2, py + 2, sz - 4, sz - 4};
    SDL_SetRenderDrawColor(r, c.r, c.g, c.b, 255);
    SDL_RenderFillRect(r, &inner);

    /* Destello superior-izquierdo (más claro) */
    uint8_t hr = (uint8_t)(c.r + 70 > 255 ? 255 : c.r + 70);
    uint8_t hg = (uint8_t)(c.g + 70 > 255 ? 255 : c.g + 70);
    uint8_t hb = (uint8_t)(c.b + 70 > 255 ? 255 : c.b + 70);
    SDL_SetRenderDrawColor(r, hr, hg, hb, 255);
    SDL_RenderDrawLine(r, px+1, py+1, px+sz-2, py+1);   /* tope */
    SDL_RenderDrawLine(r, px+1, py+1, px+1,    py+sz-2);/* izq  */

    /* Sombra inferior-derecha (más oscuro) */
    uint8_t sr = (uint8_t)(c.r / 3);
    uint8_t sg = (uint8_t)(c.g / 3);
    uint8_t sb = (uint8_t)(c.b / 3);
    SDL_SetRenderDrawColor(r, sr, sg, sb, 255);
    SDL_RenderDrawLine(r, px+sz-2, py+1,    px+sz-2, py+sz-2);/* der  */
    SDL_RenderDrawLine(r, px+1,    py+sz-2, px+sz-2, py+sz-2);/* base */

    /* Borde exterior negro */
    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    SDL_Rect outer = {px, py, sz, sz};
    SDL_RenderDrawRect(r, &outer);
}

/* ================================================================
 * PANEL LATERAL: siguiente pieza + score
 * ================================================================ */
static void draw_panel(SDL_Renderer *r, const game_t *game) {
    int panel_x = BOARD_OX + CELL_SIZE * 8 + MARGIN;
    int panel_y = BOARD_OY;

    /* Fondo del panel */
    SDL_Color pbg = (SDL_Color)COL_PANEL_BG;
    SDL_SetRenderDrawColor(r, pbg.r, pbg.g, pbg.b, 255);
    SDL_Rect panel_rect = {panel_x - 6, panel_y, PANEL_W, WIN_H - 2*MARGIN};
    SDL_RenderFillRect(r, &panel_rect);

    /* Borde izquierdo del panel */
    SDL_Color bc = (SDL_Color)COL_BORDER;
    SDL_SetRenderDrawColor(r, bc.r, bc.g, bc.b, 255);
    SDL_RenderDrawLine(r, panel_x - 6, panel_y, panel_x - 6, panel_y + WIN_H - 2*MARGIN);

    /* ---- NEXT ---- */
    SDL_Color wh = (SDL_Color)COL_WHITE;
    draw_text(r, "NEXT", panel_x, panel_y + 4, wh);

    /* Fondo de la caja "next" */
    int next_box_x = panel_x;
    int next_box_y = panel_y + 24;
    SDL_SetRenderDrawColor(r, 8, 8, 18, 255);
    SDL_Rect next_box = {next_box_x - 2, next_box_y - 2, 4*20+4, 4*20+4};
    SDL_RenderFillRect(r, &next_box);
    SDL_Color gbrd = (SDL_Color)COL_BORDER;
    SDL_SetRenderDrawColor(r, gbrd.r/2, gbrd.g/2, gbrd.b/2, 255);
    SDL_RenderDrawRect(r, &next_box);

    /* Dibuja la siguiente pieza centrada en la caja 4×4 (celdas de 20px) */
    piece_type_t nt = game_next_type(game);
    uint8_t color_idx = (uint8_t)(nt + 1u);
    piece_t preview;
    piece_init(&preview, nt, 0, 0);

    for (uint8_t row = 0; row < 4; row++) {
        for (uint8_t col = 0; col < 4; col++) {
            if (get_piece(&preview, row, col)) {
                draw_cell(r,
                    next_box_x + col * 20,
                    next_box_y + row * 20,
                    20,
                    color_idx);
            } else {
                /* celda vacía de la preview */
                SDL_SetRenderDrawColor(r, 8, 8, 18, 255);
                SDL_Rect ec = {next_box_x + col*20, next_box_y + row*20, 20, 20};
                SDL_RenderFillRect(r, &ec);
            }
        }
    }

    /* ---- SCORE ---- */
    int score_y = next_box_y + 4*20 + 28;
    draw_text(r, "SCORE", panel_x, score_y, wh);

    char sbuf[12];
    uint32_t sc = game_score(game);
    u32_to_str(sc, sbuf, sizeof(sbuf));
    SDL_Color yl = (SDL_Color)COL_YELLOW;
    draw_text(r, sbuf, panel_x, score_y + 16, yl);

    /* ---- CONTROLES (pequeño recordatorio) ---- */
    SDL_Color dim = (SDL_Color)COL_DIMWHITE;
    int help_y = score_y + 60;
    draw_text(r, "MOVE",  panel_x, help_y,      dim);
    draw_text(r, "ARROWS", panel_x, help_y+14,  dim);
    draw_text(r, "ROT",   panel_x, help_y+34,   dim);
    draw_text(r, "UP W",  panel_x, help_y+48,   dim);
    draw_text(r, "QUIT",  panel_x, help_y+68,   dim);
    draw_text(r, "ESC",   panel_x, help_y+82,   dim);
}

/* ================================================================
 * CALLBACKS DE display_port_t
 * ================================================================ */
static void sdl_begin_frame(void *ctx) {
    sdl_display_t *d = (sdl_display_t *)ctx;
    SDL_Color bg = (SDL_Color)COL_BG;
    SDL_SetRenderDrawColor(d->proxy->renderer, bg.r, bg.g, bg.b, 255);
    SDL_RenderClear(d->proxy->renderer);
}

static void sdl_draw_cell(void *ctx, uint8_t x, uint8_t y, uint8_t color) {
    sdl_display_t *d = (sdl_display_t *)ctx;
    int px = BOARD_OX + x * CELL_SIZE;
    int py = BOARD_OY + y * CELL_SIZE;
    draw_cell(d->proxy->renderer, px, py, CELL_SIZE, color);
}

static void sdl_end_frame(void *ctx) {
    sdl_display_t *d = (sdl_display_t *)ctx;
    SDL_Renderer  *r = d->proxy->renderer;

    /* Borde del tablero */
    SDL_Color bc = (SDL_Color)COL_BORDER;
    SDL_SetRenderDrawColor(r, bc.r, bc.g, bc.b, 255);
    SDL_Rect board_rect = {BOARD_OX - 1, BOARD_OY - 1,
                           CELL_SIZE * 8 + 2, CELL_SIZE * 16 + 2};
    SDL_RenderDrawRect(r, &board_rect);

    /* Panel lateral */
    draw_panel(r, d->game);

    SDL_RenderPresent(r);
}

/* ================================================================
 * ATTRACT SCREEN para SDL
 * (letras TETRIS en pixel-font cayendo, con colores arcoíris)
 * ================================================================ */

/* Fuente 3×5 de las letras T-E-T-R-I-S (reutiliza PIXEL_FONT) */
static const char ATTRACT_LETTERS[] = "TETRIS";
static const SDL_Color ATTRACT_COLORS[6] = {
    { 0,220,220,255},  /* T – cian    */
    {220,220,  0,255}, /* E – amarillo */
    {160,  0,200,255}, /* T – púrpura  */
    { 30, 80,255,255}, /* R – azul     */
    {  0,200, 60,255}, /* I – verde    */
    {220, 30, 30,255}, /* S – rojo     */
};

#define ATT_SCALE   7    /* px por píxel de letra */
#define ATT_GAP    10    /* px entre letras        */
#define ATT_LETTER_W  (FONT_CHAR_W * ATT_SCALE + ATT_GAP)
#define ATT_TOTAL_W   (6 * ATT_LETTER_W - ATT_GAP)

void sdl_attract_render(SDL_Renderer *r, int title_y, uint32_t pulse_tick) {
    /* Fondo */
    SDL_Color bg = (SDL_Color)COL_BG;
    SDL_SetRenderDrawColor(r, bg.r, bg.g, bg.b, 255);
    SDL_RenderClear(r);

    /* Letras TETRIS */
    int start_x = (WIN_W - ATT_TOTAL_W) / 2;

    for (int li = 0; li < 6; li++) {
        SDL_Color col = ATTRACT_COLORS[li];
        int lx = start_x + li * ATT_LETTER_W;
        int idx = char_to_idx(ATTRACT_LETTERS[li]);
        if (idx < 0) continue;

        for (int row = 0; row < FONT_CHAR_H; row++) {
            uint8_t bits = PIXEL_FONT[idx][row];
            for (int col_bit = 0; col_bit < FONT_CHAR_W; col_bit++) {
                if (bits & (1u << (3 - col_bit))) {
                    SDL_Rect dot = {
                        lx     + col_bit * ATT_SCALE,
                        title_y + row    * ATT_SCALE,
                        ATT_SCALE, ATT_SCALE
                    };
                    SDL_SetRenderDrawColor(r, col.r, col.g, col.b, 255);
                    SDL_RenderFillRect(r, &dot);

                    /* mini-highlight */
                    SDL_SetRenderDrawColor(r,
                        col.r/2, col.g/2, col.b/2, 255);
                    SDL_RenderDrawLine(r,
                        dot.x, dot.y+dot.h-1,
                        dot.x+dot.w-1, dot.y+dot.h-1);
                }
            }
        }
    }

    /* "PRESS ANY KEY" pulsante */
    uint8_t bright = (uint8_t)(100 + (pulse_tick % 60) * 2);
    if (pulse_tick % 60 > 30)
        bright = (uint8_t)(100 + (60 - pulse_tick % 60) * 2);
    SDL_Color pc = {bright, bright, bright+40, 255};

    int msg_x = (WIN_W - 14*(FONT_CHAR_W*FONT_SCALE + FONT_GAP)) / 2;
    draw_text(r, "PRESS ANY KEY", msg_x, WIN_H - 40, pc);

    SDL_RenderPresent(r);
}

/* Pantalla de GAME OVER + score */
void sdl_score_render(SDL_Renderer *r, uint32_t score) {
    SDL_Color bg = (SDL_Color)COL_BG;
    SDL_SetRenderDrawColor(r, bg.r, bg.g, bg.b, 255);
    SDL_RenderClear(r);

    int cx = WIN_W / 2;
    int cy = WIN_H / 2;

    /* Caja central */
    SDL_Color bc = (SDL_Color)COL_BORDER;
    SDL_SetRenderDrawColor(r, 20, 20, 45, 255);
    SDL_Rect box = {cx - 100, cy - 60, 200, 120};
    SDL_RenderFillRect(r, &box);
    SDL_SetRenderDrawColor(r, bc.r, bc.g, bc.b, 255);
    SDL_RenderDrawRect(r, &box);

    SDL_Color wh  = (SDL_Color)COL_WHITE;
    SDL_Color yl  = (SDL_Color)COL_YELLOW;

    int gox = cx - (9*(FONT_CHAR_W*FONT_SCALE+FONT_GAP))/2;
    draw_text(r, "GAME OVER", gox, cy - 48, wh);

    draw_text(r, "SCORE", cx - 30, cy - 16, wh);

    char sbuf[12];
    u32_to_str(score, sbuf, sizeof(sbuf));
    int sw = (int)strlen(sbuf) * (FONT_CHAR_W*FONT_SCALE + FONT_GAP);
    draw_text(r, sbuf, cx - sw/2, cy + 6, yl);

    SDL_Color dim = (SDL_Color)COL_DIMWHITE;
    int con_x = cx - (15*(FONT_CHAR_W*FONT_SCALE+FONT_GAP))/2;
    draw_text(r, "CONTINUING IN...", con_x, cy + 40, dim);

    SDL_RenderPresent(r);
}

/* ================================================================
 * CONSTRUCTOR DEL ADAPTER
 * ================================================================ */
display_port_t sdl_display_create(sdl_display_t *d,
                                  sdl_proxy_t   *proxy,
                                  const game_t  *game) {
    d->proxy = proxy;
    d->game  = game;

    display_port_t port;
    port.begin_frame = sdl_begin_frame;
    port.draw_cell   = sdl_draw_cell;
    port.end_frame   = sdl_end_frame;
    port.ctx         = d;
    return port;
}