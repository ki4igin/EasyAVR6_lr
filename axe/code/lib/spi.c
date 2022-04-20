
// Includes --------------------------------------------------------------------
#include "spi.h"
#include <avr/interrupt.h>

// Typedef ---------------------------------------------------------------------
// Тип статуса модуля SPI

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

#define SPI_CS_AXE 4  // Номер вывода МК к которому подключен вывод CS акселерометра

volatile enum spi_status spi_status = SPI_STATUS_NOT_INIT;

static struct spi_buf spi_buf;

// Functions -------------------------------------------------------------------
/*******************************************************************************
Функция инициализации SPI
По окончанию инициализации устанавливается статус SPI_STATUS_READY
Скорость обмена: F_CPU / 4
Режим: Master; CPOL = 0; CPHA = 0
Разрешение прерывания по завершению обмена
*******************************************************************************/

void spi_init(void)
{
    SPI_DDR |= (1 << SPI_SCK) | (1 << SPI_MOSI) | (1 << SPI_CS_AXE);
    SPI_DDR &= ~(1 << SPI_MISO);

    SPI_PORT |= (1 << SPI_CS_AXE);

    SPCR = (1 << SPIE) | (1 << SPE) | (1 << MSTR);

    spi_status = SPI_STATUS_READY;
}

/*******************************************************************************
Функция обмена данными с ведомым устройством
Во время обмена устанавливается статус SPI_STATUS_BUSY, после обмена устанавливается
статус SPI_STATUS_READY

Аргументы:
buf        указатель на буфер для обмена
size     размер буфера для обмена
*******************************************************************************/
void spi_txrx(void *buf, uint8_t size)
{
    // Если статус модуля не равен SPI_STATUS_READY, то выходим из функции
    if (spi_status != SPI_STATUS_READY)
    {
        return;
    }

    // Установка статуса SPI_STATUS_BUSY
    spi_status = SPI_STATUS_BUSY;

    // Сброс вывода СS (выбор ведомого устройства)
    SPI_PORT &= ~(1 << SPI_CS_AXE);

    // Установка размера и указателя на буфер обмена и
    // передача первого байта данных из буфера обмена
    spi_buf.count = size;
    spi_buf.data = buf;
    SPDR = *spi_buf.data;
}

/*******************************************************************************
Функция обработки события по завершению обмена одним байтом модуля SPI
Принятые данные помещаются в буфер обмена вместо переданных данных
*******************************************************************************/
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
        SPI_PORT |= (1 << SPI_CS_AXE);
        spi_status = SPI_STATUS_READY;
    };
}

/* End File */