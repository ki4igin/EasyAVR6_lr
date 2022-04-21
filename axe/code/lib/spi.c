#include <avr/interrupt.h>

#include "spi.h"

struct spi_buf
{
    uint8_t *data;
    uint8_t count;
};

#define SPI_PORT PORTB  // Порт на который выведен модуль SPI
#define SPI_DDR  DDRB   //
#define SPI_PIN  PINB   //

#define SPI_SCK  7  // Номер вывода SCK модуля SPI0
#define SPI_MISO 6  // Номер вывода MISO модуля SPI
#define SPI_MOSI 5  // Номер вывода MOSI модуля SPI
#define SPI_SS   4  // Номер вывода SS модуля SPI

#define SPI_CS 4  // Номер вывода к которому подключен вывод CS акселерометра

volatile enum spi_status spi_status = SPI_STATUS_NOT_INIT;

static struct spi_buf spi_buf;

/**
 * Функция инициализации SPI
 * По окончанию инициализации устанавливается статус SPI_STATUS_RDY
 * Скорость обмена: F_CPU / 64
 * Режим: Master; CPOL = 0; CPHA = 0
 * Разрешение прерывания по завершению обмена
 */
void spi_init(void)
{
    SPI_DDR |= (1 << SPI_SCK) | (1 << SPI_MOSI) | (1 << SPI_CS);
    SPI_DDR &= ~(1 << SPI_MISO);

    SPI_PORT |= (1 << SPI_CS) | (1 << SPI_SS);

    SPCR = (1 << SPIE) | (1 << SPE) | (1 << MSTR) | (1 << SPR1);

    spi_status = SPI_STATUS_RDY;
}

/**
 * Функция обмена данными с ведомым устройством
 * Во время обмена устанавливается статус SPI_STATUS_BUSY, после обмена 
 * устанавливается статус SPI_STATUS_RDY
 * 
 * \param buf на буфер для обмена
 * \param size размер буфера для обмена
 */
void spi_txrx(void *buf, uint8_t size)
{
    if (spi_status != SPI_STATUS_RDY)
    {
        return;
    }

    spi_status = SPI_STATUS_BUSY;

    SPI_PORT &= ~(1 << SPI_CS);

    spi_buf.count = size;
    spi_buf.data = buf;
    SPDR = *spi_buf.data;
}

/* Прерывание используется для обмена пакетом данных */
ISR(SPI_STC_vect)
{
    *spi_buf.data = SPDR;

    if (--spi_buf.count)
    {
        spi_buf.data++;
        SPDR = *spi_buf.data;
    }
    else
    {
        SPI_PORT |= (1 << SPI_CS);
        spi_status = SPI_STATUS_RDY;
    };
}

/* End File */