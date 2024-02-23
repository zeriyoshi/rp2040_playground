<?php

const GAMMA = 7.2;

?>#ifndef _LED_GAMMA_H
# define _LED_GAMMA_H

#include <stdio.h>

const uint16_t _gamma_lut_16[] = {<?php
for ($i = 0; $i <= 65535; $i++) {
    if ($i > 0) {
        echo ", ";
    }
    if (($i % 16) === 0) {
        echo \PHP_EOL;
    }
    echo (int)(pow((float) $i / (float) 65535, GAMMA) * 65535 + 0.5);
}
?>};
const uint8_t _gamma_lut_8[] = {<?php
for ($i = 0; $i <= 255; $i++) {
    if ($i > 0) {
        echo ", ";
    }
    if (($i % 16) === 0) {
        echo \PHP_EOL;
    }
    echo (int)(pow((float) $i / (float) 255, GAMMA) * 255 + 0.5);
}
?>};

#endif
