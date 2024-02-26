#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

#include "led/color.h"
#include "neopixel.pio.h"

#include "proprietary/jcdk_images.h"

#define BRIGHTNESS 0.5

static inline uint32_t rgb2grb(uint8_t r, uint8_t g, uint8_t b) {
    return
            ((uint32_t) (r) << 8) |
            ((uint32_t) (g) << 16) |
            (uint32_t) (b);
}

static inline void send_map(uint32_t data) {
    pio_sm_put_blocking(pio0, 0, (rgb2grb(
        brightness8(gamma8((data >> 16) & 0xFF), BRIGHTNESS),
        brightness8(gamma8((data >> 8) & 0xFF), BRIGHTNESS),
        brightness8(gamma8(data & 0xFF), BRIGHTNESS)
     ) << 8U));
}

void draw(const uint32_t data[]) {
    uint8_t i = 0;

    for (int row = 0; row < IMAGES_ROWS; row++) {
        if (row % 2 == 0) {
            for (int col = IMAGES_COLS - 1; col >= 0; col--) {
                send_map(data[row * IMAGES_COLS + col]);
            }
        } else {
            for (int col = 0; col < IMAGES_COLS; col++) {
                send_map(data[row * IMAGES_COLS + col]);
            }
        }
    }
}

int main() {
    stdio_init_all();

    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &neopixel_program);

    neopixel_program_init(pio, sm, offset, 2, 800000, false);

    while (true) {
        for (uint8_t i = 0; i < IMAGES_NUM; ++i) {
            draw(images[i]);
            sleep_ms(1000);
        }
    }

    return 0;
}
