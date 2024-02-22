#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#include "led/color.h"

/*
 * For Pimoroni Tiny2040
 */
#define LED_R 18
#define LED_G 19
#define LED_B 20

int main() {
    rgb48_t rgb;
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
            rgb = hsvTo48BitRgb(hue, 65535, 65535);

            pwm_set_gpio_level(LED_R, rgb.r);
            pwm_set_gpio_level(LED_G, rgb.g);
            pwm_set_gpio_level(LED_B, rgb.b);
            sleep_ms(50);
        }
    }

    return 0;
}
