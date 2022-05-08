#ifndef CONVERTERS_H
#define CONVERTERS_H

#include <avr/pgmspace.h>

/**
 * Функция преобразования знакового десятичного числа с фиксированной точкой
 * в строку
 * 
 * \param value знаковое число
 * \param pos_point позиция десятичной точки
 * \param buf указатель на буфер результирующей строки
 * \param len длина строки = знак числа + необходимое кол-во цифр числа + точка
 * 
 * пример: int2str(12345, 2, buf[5], 5) => buf[5] = "+12.3"
 */

static inline void int2str(
    int16_t value, uint8_t pos_point, uint8_t *buf, uint8_t len)
{
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

static inline uint8_t uint2str(
    uint16_t value, uint8_t *buf)
{
    PROGMEM static const uint16_t dividers[] = {10000, 1000, 100, 10, 1};

    if (value == 0)
    {
        *buf = '0';
        return 1;
    }

    uint8_t new_len = 0;
    uint8_t value_old = value;


    for (uint8_t i = 0; i < (sizeof(dividers) / sizeof(dividers[0])); i++)
    {
        uint16_t divider = pgm_read_word(&dividers[i]);

        if (value_old < divider)
        {
            continue;
        }

        *buf++ = value / divider + '0';
        value %= divider;
        new_len++;
    }

    return new_len;
}

#endif

/* End File */