#include <dual_matrix_proxy.h>

/*
  Convención lógica:
  - rows[0..7]   = matriz superior
  - rows[8..15]  = matriz inferior

  Cada rows[y] usa 8 bits para las 8 columnas.
  Bit 0 -> x = 0
  Bit 1 -> x = 1
  ...
  Bit 7 -> x = 7

  Si un bit vale 1, el pixel está ENCENDIDO a nivel lógico.
*/

void dual_matrix_proxy_init(dual_matrix_proxy_t *self, hc595_proxy_t *chain) {
    self->chain = chain;
    self->scan_row = 0u;
    dual_matrix_proxy_clear(self);
}

void dual_matrix_proxy_clear(dual_matrix_proxy_t *self) {
    uint8_t y;

    for (y = 0u; y < DUAL_MATRIX_HEIGHT; y++) {
        self->rows[y] = 0u;
    }
}

void dual_matrix_proxy_set_pixel(dual_matrix_proxy_t *self, uint8_t x, uint8_t y, uint8_t on) {
    uint8_t mask;

    if (x >= DUAL_MATRIX_WIDTH || y >= DUAL_MATRIX_HEIGHT) {
        return;
    }

    mask = (uint8_t)(1u << x);

    if (on) {
        self->rows[y] |= mask;
    } else {
        self->rows[y] &= (uint8_t)(~mask);
    }
}

uint8_t dual_matrix_proxy_get_pixel(const dual_matrix_proxy_t *self, uint8_t x, uint8_t y) {
    uint8_t mask;

    if (x >= DUAL_MATRIX_WIDTH || y >= DUAL_MATRIX_HEIGHT) {
        return 0u;
    }

    mask = (uint8_t)(1u << x);
    return (self->rows[y] & mask) ? 1u : 0u;
}

void dual_matrix_proxy_scan_step(dual_matrix_proxy_t *self) {
    uint8_t row;
    uint8_t fila_superior;
    uint8_t fila_inferior;
    uint8_t columnas_superior;
    uint8_t columnas_inferior;

    row = self->scan_row;

    fila_superior = (uint8_t)(1u << row);
    fila_inferior = (uint8_t)(1u << row);

    columnas_superior = (uint8_t)(~self->rows[row]);
    columnas_inferior = (uint8_t)(~self->rows[row + 8u]);

    hc595_proxy_write(
        self->chain,
        columnas_inferior,
        fila_inferior,
        columnas_superior,
        fila_superior
    );

    self->scan_row = (uint8_t)((self->scan_row + 1u) % 8u);
}