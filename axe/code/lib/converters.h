#ifndef CONVERTERS_H
#define CONVERTERS_H

#include <avr/pgmspace.h>

static inline void int2str(
    int16_t value, uint8_t pos_point, uint8_t *buf, uint8_t len)
{
    /* Установка указателя на последний (старший) разряд */
    if (value > 0)
    {
        *buf++ = '+';
    }
    else
    {
        *buf++ = '-';
        value = -value;
    }

    PROGMEM static const uint16_t dividers[] = {10000, 1000, 100, 10, 1};

    for (uint8_t i = 0; i < len - 2; i++)
    {
        if (i == pos_point)
        {
            *buf++ = '.';
        }

        uint16_t divider = pgm_read_word(&dividers[i]);

        *buf++ = value / divider + '0';
        value %= divider;
    }
}

#endif

/* End File */