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
    rgb48_t rgb = rgb24_to_rgb48(color_rgb24(208, 74, 34));
    pwm_config config = pwm_get_default_config();

    gpio_set_function(LED_R, GPIO_FUNC_PWM);
    gpio_set_function(LED_G, GPIO_FUNC_PWM);
    gpio_set_function(LED_B, GPIO_FUNC_PWM);

    pwm_config_set_clkdiv(&config, 6.1f);

    pwm_init(pwm_gpio_to_slice_num(LED_R), &config, true);
    pwm_init(pwm_gpio_to_slice_num(LED_G), &config, true);
    pwm_init(pwm_gpio_to_slice_num(LED_B), &config, true);

    apply_gamma_rgb48(&rgb);

    pwm_set_gpio_level(LED_R, ~rgb.r + 1);
    pwm_set_gpio_level(LED_G, ~rgb.g + 1);
    pwm_set_gpio_level(LED_B, ~rgb.b + 1);
    sleep_us(600);

    return 0;
}
