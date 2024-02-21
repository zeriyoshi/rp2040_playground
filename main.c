#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

/*
 * For Pimoroni Tiny2040
 */
#define LED_R 18
#define LED_G 19
#define LED_B 20

typedef struct {
    uint16_t r;
    uint16_t g;
    uint16_t b;
} rgb_t;

/* From Adafruit_NeoPixel (https://github.com/adafruit/Adafruit_NeoPixel) */
rgb_t color_hsv(uint16_t hue, uint8_t sat, uint8_t val) {
    uint8_t r, g, b, s2;
    uint16_t s1;
    uint32_t v1;

    hue = (hue * 1530L + 32768) / 65536;
    if (hue < 510) {
        b = 0;
        if (hue < 255) {
            r = 255;
            g = hue;
        } else {
            r = 510 - hue;
            g = 255;
        }
    } else if (hue < 1020) {
        r = 0;
        if (hue <  765) {
            g = 255;
            b = hue - 510;
        } else {
            g = 1020 - hue;
            b = 255;
        }
    } else if (hue < 1530) {
        g = 0;
        if (hue < 1275) {
            r = hue - 1020;
            b = 255;
        } else {
            r = 255;
            b = 1530 - hue;
        }
    } else {
        r = 255;
        g = b = 0;
    }

    v1 = 1 + val;
    s1 = 1 + sat;
    s2 = 255 - sat;

    rgb_t color = {
        (uint16_t) (((((r * s1) >> 8) + s2) * v1) & 0xff00),
        (uint16_t) (((((g * s1) >> 8) + s2) * v1) & 0xff00),
        (uint16_t) ((((b * s1) >> 8) + s2) * v1)
    };

    return color;
}

int main() {
    rgb_t rgb;
    pwm_config config;

    gpio_set_function(LED_R, GPIO_FUNC_PWM);
    gpio_set_function(LED_G, GPIO_FUNC_PWM);
    gpio_set_function(LED_B, GPIO_FUNC_PWM);

    config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.f);

    pwm_init(pwm_gpio_to_slice_num(LED_R), &config, true);
    pwm_init(pwm_gpio_to_slice_num(LED_G), &config, true);
    pwm_init(pwm_gpio_to_slice_num(LED_B), &config, true);

    while (true) {
        for (uint16_t hue = 0; hue < 65535; hue += 1000) {
            for (uint8_t i = 0; i < 64; i++) {
                rgb = color_hsv(
                    hue + (i * 65536) / 64,
                    255,
                    255
                );
                pwm_set_gpio_level(LED_R, rgb.r);
                pwm_set_gpio_level(LED_G, rgb.g);
                pwm_set_gpio_level(LED_B, rgb.b);
                sleep_ms(50);
            }
        }
    }

    return 0;
}
