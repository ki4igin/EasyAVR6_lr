#ifndef CHECKSUM_H
#define CHECKSUM_H

#include "avr/io.h"

/**
 * Функция вычисления контрольной суммы массива
 * 
 * \param buf указатель на буфер
 * \param size размер буфера
 * 
 * \return значение контрольной суммы
 * *******************************************************************************/
static inline uint8_t check_sum_calc(const uint8_t *buf, uint8_t size)
{
    uint8_t sum = 0;
    for (uint8_t i = 0; i < size; i++)
    {
        sum += *buf++;
    }
    return sum;
}

#endif

/* End File */