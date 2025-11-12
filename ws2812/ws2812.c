#include "hardware/pio.h"
#include "pico/stdlib.h"
#include "ws2812.pio.h"
#include <stdio.h>

#define LED_PIN 0
#define NUM_PIXELS 64
#define IS_RGBW false

#define TOUCH_PIN 22

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

void gradiant(uint8_t r_start, uint8_t g_start, uint8_t b_start, uint8_t r_end,
              uint8_t g_end, uint8_t b_end, float brightness) {
    float distance_r1 = (r_end - r_start) / (float)NUM_PIXELS;
    float distance_g1 = (g_end - g_start) / (float)NUM_PIXELS;
    float distance_b1 = (b_end - b_start) / (float)NUM_PIXELS;

    for (int i = 0; i < NUM_PIXELS; i++) { // blue to purple
        put_pixel(urgb_u32(scale(brightness, r_start + (distance_r1 * i)),
                           scale(brightness, g_start + (distance_g1 * i)),
                           scale(brightness, b_start + (distance_b1 * i))));
    }
}

int main() {
    stdio_init_all();
    gpio_init(TOUCH_PIN);
    gpio_set_dir(TOUCH_PIN, GPIO_IN);

    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, LED_PIN, 800000, IS_RGBW);

    float brightness = 0.1f;

    uint8_t r1 = 0, g1 = 15, b1 = 145;
    uint8_t r2 = 181, g2 = 0, b2 = 201;

    // dark blue 0, 15, 145
    // purple 181, 0, 201

    uint8_t r3 = 255, g3 = 0, b3 = 0;
    uint8_t r4 = 255, g4 = 0, b4 = 144;

    // red 255, 0, 0
    // pink 255, 0, 144

    bool touch = 0;
    bool color_switch = false;
    int timeout = 0;

    while (true) {
        touch = gpio_get(TOUCH_PIN);
        if (touch) {
            color_switch = !color_switch;
            printf("Colors switched \n");
        } else {
            timeout++;
        }
        if (timeout == 150) { // turn led off after 30s
            timeout = 0;
            lights_off();
            while (!gpio_get(TOUCH_PIN)) {
                printf("paused \n");
            };
            // pause programm
        }

        if (color_switch) {
            for (int i = 0; i < NUM_PIXELS; i++) { // blue to purple
                gradiant(r1, g1, b1, r2, g2, b2, brightness);
            }
        }
        if (gpio_get(TOUCH_PIN)) { // red to pink
            for (int i = 0; i < NUM_PIXELS; i++) {
                gradiant(r3, g3, b3, r4, g4, b4, brightness);
            }
        }
        sleep_ms(200);
    }
}
