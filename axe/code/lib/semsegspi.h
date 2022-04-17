#ifndef __SEMSEGSPI_H
#define __SEMSEGSPI_H

// Includes --------------------------------------------------------------------
#include <avr/io.h>        // Заголовочный файл для работы с РВВ МК
#include <avr/pgmspace.h>  // Заголовчный файл для работы с памятью программ МК

#include "spi.h"  // Заголовочный файл для работы с модулем SPI МК

// Function prototypes ---------------------------------------------------------
void SemSegInit(void);
void SemSegDisp(uint8_t* pbufDisp, uint8_t bufDispSize);
void SemsegBin2Bcd(uint16_t data, uint8_t* pbuf, uint8_t bufSize);

#endif
// End File --------------------------------------------------------------------