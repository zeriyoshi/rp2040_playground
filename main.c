#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

#include "led/color.h"
#include "neopixel.pio.h"

#include "jcdk.h"

static inline uint32_t rgb2grb(uint8_t r, uint8_t g, uint8_t b) {
    return
            ((uint32_t) (r) << 8) |
            ((uint32_t) (g) << 16) |
            (uint32_t) (b);
}

static inline void send_map(uint32_t data) {
    pio_sm_put_blocking(pio0, 0, (rgb2grb(
        gamma8((data >> 16) & 0xFF),
        gamma8((data >> 8) & 0xFF),
        gamma8(data & 0xFF)
     ) << 8U));
}

void draw(const uint32_t data[]) {
    for (int row = 0; row < 8; row++) {
        if (row % 2 == 0) {
            for (int col = 8 - 1; col >= 0; col--) {
                int index = row * 8 + col;
                send_map(data[index]);
            }
        } else {
            for (int col = 0; col < 8; col++) {
                int index = row * 8 + col;
                send_map(data[index]);
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
        draw(yurine);
        sleep_ms(1000);
        draw(jashin);
        sleep_ms(1000);
        draw(jashin2);
        sleep_ms(1000);
    }

    return 0;
}
