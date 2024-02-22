#ifndef _LED_COLOR_H
#define _LED_COLOR_H

#include <stdio.h>

#include "gamma.h"

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb24_t;

typedef struct {
    uint16_t r;
    uint16_t g;
    uint16_t b;
} rgb48_t;

uint16_t inline map8to16(uint8_t n) {
    uint16_t r = (uint16_t) n << 8;
    if (n == 255) {
        r += 255;
    }
    return r;
}

uint8_t inline map16to8(uint16_t n) {
    return (uint8_t) n >> 8;
}

rgb48_t inline color_rgb48(uint16_t r, uint16_t g, uint16_t b) {
    rgb48_t c = {r, g, b};
    return c;
}

rgb24_t inline color_rgb24(uint8_t r, uint8_t g, uint8_t b) {
    rgb24_t c = {r, g, b};
    return c;
}

rgb48_t rgb24_to_rgb48(rgb24_t rgb) {
    return color_rgb48(
        map8to16(rgb.r),
        map8to16(rgb.g),
        map8to16(rgb.b)
    );
}

rgb24_t rgb48_to_rgb24(rgb48_t rgb) {
    return color_rgb24(
        map16to8(rgb.r),
        map16to8(rgb.g),
        map16to8(rgb.b)
    );
}

uint8_t inline gamma8(uint8_t n) {
    return _gamma_lut_8[n];
}

uint16_t inline gamma16(uint16_t n) {
    return _gamma_lut_16[n];
}

void apply_gamma_24(rgb24_t *rgb) {
    rgb->r = gamma8(rgb->r);
    rgb->g = gamma8(rgb->g);
    rgb->b = gamma8(rgb->b);
}

void apply_gamma_48(rgb48_t *rgb) {
    rgb->r = gamma16(rgb->r);
    rgb->g = gamma16(rgb->g);
    rgb->b = gamma16(rgb->b);
}

rgb24_t hsv_to_rgb24(uint8_t hue, uint8_t sat, uint8_t val) {
    uint8_t region, remainder, p, q, t;

    if (sat == 0) {
        return color_rgb24(val, val, val);
    }

    region = hue / 43;
    remainder = (hue - (region * 43)) * 6;

    p = (val * (255 - sat)) / 255;
    q = (val * (255 - ((sat * remainder) / 255))) / 255;
    t = (val * (255 - ((sat * (255 - remainder)) / 255))) / 255;

    switch (region) {
        case 0:
            return color_rgb24(val, t, p);
        case 1:
            return color_rgb24(q, val, p);
        case 2:
            return color_rgb24(p, val, t);
        case 3:
            return color_rgb24(p, q, val);
        case 4:
            return color_rgb24(t, p, val);
        default:
            return color_rgb24(val, p, q);
    }
}

rgb48_t hsv_to_rgb48(uint16_t hue, uint16_t sat, uint16_t val) {
    unsigned int region, remainder, p, q, t;
    unsigned long hue_scaled = (uint16_t)hue * 360 / 65535;

    if (sat == 0) {
        return color_rgb48(val, val, val);
    }

    region = hue_scaled / 60;
    remainder = ((hue_scaled % 60) * 65535) / 60;

    p = (val * (65535 - sat)) >> 16;
    q = (val * (65535 - ((sat * remainder) / 65535))) >> 16;
    t = (val * (65535 - ((sat * (65535 - remainder)) / 65535))) >> 16;

    switch (region) {
        case 0:
            return color_rgb48(val, t, p);
        case 1:
            return color_rgb48(q, val, p);
        case 2:
            return color_rgb48(p, val, t);
        case 3:
            return color_rgb48(p, q, val);
        case 4:
            return color_rgb48(t, p ,val);
        default:
            return color_rgb48(val, p, q);
    }
}

#endif
