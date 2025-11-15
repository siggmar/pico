#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <string.h>

// SSD1306 Parameter
#define I2C_PORT i2c1
#define SDA_PIN 0
#define SCL_PIN 1
#define SSD1306_ADDR 0x3D
#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64

// SSD1306 Befehle
#define SSD1306_CMD 0x00
#define SSD1306_DATA 0x40

// Ein paar einfache Zeichenmuster (5x7 ASCII)
extern const uint8_t font5x7[96][5];

// --- I2C Hilfsfunktionen ---
void ssd1306_write_cmd(uint8_t cmd) {
    uint8_t buf[2] = {SSD1306_CMD, cmd};
    i2c_write_blocking(I2C_PORT, SSD1306_ADDR, buf, 2, false);
}

void ssd1306_write_data(uint8_t *data, size_t len) {
    uint8_t buf[len + 1];
    buf[0] = SSD1306_DATA;
    memcpy(buf + 1, data, len);
    i2c_write_blocking(I2C_PORT, SSD1306_ADDR, buf, len + 1, false);
}

// --- Display Initialisierung ---
void ssd1306_init() {
    ssd1306_write_cmd(0xAE); // Display aus
    ssd1306_write_cmd(0xD5);
    ssd1306_write_cmd(0x80);
    ssd1306_write_cmd(0xA8);
    ssd1306_write_cmd(0x3F);
    ssd1306_write_cmd(0xD3);
    ssd1306_write_cmd(0x00);
    ssd1306_write_cmd(0x40 | 0x00);
    ssd1306_write_cmd(0x8D);
    ssd1306_write_cmd(0x14);
    ssd1306_write_cmd(0x20);
    ssd1306_write_cmd(0x00);
    ssd1306_write_cmd(0xA1);
    ssd1306_write_cmd(0xC8);
    ssd1306_write_cmd(0xDA);
    ssd1306_write_cmd(0x12);
    ssd1306_write_cmd(0x81);
    ssd1306_write_cmd(0xCF);
    ssd1306_write_cmd(0xD9);
    ssd1306_write_cmd(0xF1);
    ssd1306_write_cmd(0xDB);
    ssd1306_write_cmd(0x40);
    ssd1306_write_cmd(0xA4);
    ssd1306_write_cmd(0xA6);
    ssd1306_write_cmd(0xAF); // Display an
}

// --- Hilfsfunktionen ---
void ssd1306_clear() {
    uint8_t buf[SSD1306_WIDTH];
    memset(buf, 0x00, sizeof(buf));

    for (uint8_t page = 0; page < 8; page++) {
        ssd1306_write_cmd(0xB0 + page);
        ssd1306_write_cmd(0x00);
        ssd1306_write_cmd(0x10);
        ssd1306_write_data(buf, SSD1306_WIDTH);
    }
}

void ssd1306_draw_char(uint8_t x, uint8_t page, char c) {
    if (c < 32 || c > 127)
        c = '?';
    ssd1306_write_cmd(0xB0 + page);
    ssd1306_write_cmd(0x00 + (x & 0x0F));
    ssd1306_write_cmd(0x10 + (x >> 4));

    ssd1306_write_data((uint8_t *)font5x7[c - 32], 5);
}

void ssd1306_draw_string(uint8_t x, uint8_t page, const char *str) {
    while (*str) {
        ssd1306_draw_char(x, page, *str++);
        x += 6; // 5 Pixel + 1 Abstand
    }
}

// --- Hauptprogramm ---
int main() {
    stdio_init_all();

    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    printf("Initialisiere SSD1306...\n");
    ssd1306_init();
    ssd1306_clear();

    ssd1306_draw_string(0, 0, "A");
    ssd1306_draw_string(0, 2, "SSD1306 OLED");
    ssd1306_draw_string(0, 4, "I2C Example");

    printf("Fertig.\n");

    while (1) {
        sleep_ms(1000);
    }
}

const uint8_t font5x7[96][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, // (space)
    {0x00, 0x00, 0x5F, 0x00, 0x00}, // !
    {0x00, 0x07, 0x00, 0x07, 0x00}, // "
    {0x14, 0x7F, 0x14, 0x7F, 0x14}, // #
    // ... mehr Zeichen wenn du willst
};
