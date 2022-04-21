#ifndef SPI_H
#define SPI_H

#include <avr/io.h>

enum spi_status
{
    SPI_STATUS_NOT_INIT,
    SPI_STATUS_RDY,
    SPI_STATUS_BUSY
};

extern volatile enum spi_status spi_status;

void spi_init(void);
void spi_txrx(void* buf, uint8_t size);

#endif

/* End File */