#ifndef DUAL_MATRIX_PROXY_H
#define DUAL_MATRIX_PROXY_H

#include <stdint.h>
#include <hc595_proxy.h>

#define DUAL_MATRIX_WIDTH   8u
#define DUAL_MATRIX_HEIGHT 16u
#define DUAL_MATRIX_SCAN_ROWS 8u

typedef struct {
    hc595_proxy_t *chain;
    uint8_t rows[DUAL_MATRIX_HEIGHT];
    uint8_t scan_row;
} dual_matrix_proxy_t;

void dual_matrix_proxy_init(dual_matrix_proxy_t *self, hc595_proxy_t *chain);
void dual_matrix_proxy_clear(dual_matrix_proxy_t *self);
void dual_matrix_proxy_set_pixel(dual_matrix_proxy_t *self, uint8_t x, uint8_t y, uint8_t on);
uint8_t dual_matrix_proxy_get_pixel(const dual_matrix_proxy_t *self, uint8_t x, uint8_t y);
void dual_matrix_proxy_scan_step(dual_matrix_proxy_t *self);

#endif