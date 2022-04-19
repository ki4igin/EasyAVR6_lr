#ifndef SPI_H
#define SPI_H

// Includes --------------------------------------------------------------------
#include <avr/io.h>  // Заголовочный файл для работы с РВВ МК

enum spi_status
{
    SPI_STATUS_NOT_INIT,
    SPI_STATUS_READY,
    SPI_STATUS_BUSY
};

extern volatile enum spi_status spi_status;

// Function prototypes ---------------------------------------------------------
void spi_init(void);
void spi_txrx(uint8_t* buf, uint8_t size);

#endif
// End File --------------------------------------------------------------------