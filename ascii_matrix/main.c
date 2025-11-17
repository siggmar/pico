#include "board.pio.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"
#include <stdio.h>

#define LED_PIN 0
#define NUM_PIXELS 64
#define IS_RGBW false

static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)r << 8) | ((uint32_t)g << 16) | (uint32_t)b;
}

uint8_t scale(float brightness, uint8_t color) {
    return (uint8_t)(brightness * color);
}

void lights_off() {
    for (int i = 0; i < NUM_PIXELS; i++) {
        put_pixel(urgb_u32(0, 0, 0));
    }
}

int main() {
    stdio_init_all();

    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, LED_PIN, 800000, IS_RGBW);

    float brightness = 1.0f;

    int t = 0;

    while (true) {
        int c = getchar_timeout_us(0);
        if (c != PICO_ERROR_TIMEOUT) {
            putchar(c); // Echo
        }
    }
}
