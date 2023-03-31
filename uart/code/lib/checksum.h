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
 */
static inline uint8_t checksum_calc(const uint8_t *buf, uint8_t size)
{
    uint8_t sum = 0;
    for (uint8_t i = 0; i < size; i++)
    {
        sum += *buf++;
    }
    return sum;
}

/**
 * Функция проверки контрольной суммы массива
 * 
 * \param buf указатель на буфер
 * \param size размер буфера
 * 
 * \return 1 - если сумма сошлась, 0 - если не сошлась
 */
static inline uint8_t checksum_is_valid(const uint8_t *buf, uint8_t size)
{    
    uint8_t sum = checksum_calc(buf, size - 1);
    return buf[size - 1] == sum ? 1 : 0;
}

#endif

/* End File */