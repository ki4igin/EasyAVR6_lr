// Includes --------------------------------------------------------------------
#include "spi.h"

// Typedef ---------------------------------------------------------------------
// Тип статуса модуля SPI
typedef enum
{
    SPI_NOINIT = 0,  // SPI не инициализирован
    SPI_READY,       // SPI готов к обмену
    SPI_BUSY         // SPI занят
} SpiStatus_t;

// Macro -----------------------------------------------------------------------
#define SPI_PORT PORTB  // Порт на который выведен модуль SPI
#define SPI_DDR  DDRB   //
#define SPI_PIN  PINB   //

#define SPI_SCK  7  // Номер вывода SCK модуля SPI0
#define SPI_MISO 6  // Номер вывода MISO модуля SPI
#define SPI_MOSI 5  // Номер вывода MOSI модуля SPI
#define SPI_SS   4  // Номер вывода SS модуля SPI

#define SPI_CS 1  // Номер вывода МК к которому подключен вывод CS 
                  // расширителя портов

// Variables -------------------------------------------------------------------
static SpiStatus_t spiStatus = SPI_NOINIT;  // Переменная статуса модуля SPI
static uint8_t*    pbufSpi;                 // Указатель на буфер для обмена
static uint8_t     bufSizeSpi;              // Размер буфера для обмена

// Functions -------------------------------------------------------------------
/*******************************************************************************
Функция инициализации SPI
По окончанию инициализации устанавливается статус SPI_READY
Скорость обмена: F_CPU / 4
Режим: Master; CPOL = 0; CPHA = 0
Разрешение прерывания по завершению обмена
*******************************************************************************/
void SpiInit2(void)
{
    // Инициализация выводов:
    // SCK, MOSI, SS и CS - выходы (SS устанавливается как выход чтобы
    // модуль SPI случайно не перешел в режим Slave);
    // MISO - вход
    SPI_DDR |= (1 << SPI_SCK) | (1 << SPI_MOSI) | (1 << SPI_SS) | (1 << SPI_CS);
    SPI_DDR &= ~(1 << SPI_MISO);

    // Установка вывода СS в лог. 1
    SPI_PORT |= (1 << SPI_CS);

    // Установка режима, включение модуля SPI и прерывания по завершению обмена
    SPCR = (1 << SPIE) | (1 << SPE) | (1 << MSTR);

    // Переключение вывода СS для завершения инициализации
    SPI_PORT &= ~(1 << SPI_CS);
    SPI_PORT |= (1 << SPI_CS);

    // Установка статуса SPI_READY
    spiStatus = SPI_READY;
}

void SpiInit(void)
{
    SPI_DDR |= (1 << SPI_SCK) | (1 << SPI_MOSI) | (1 << SPI_SS) | (1 << SPI_CS);
    SPI_DDR &= ~(1 << SPI_MISO);

    SPI_PORT |= (1 << SPI_CS);
    SPI_PORT |= (1 << SPI_SS);

    SPCR = (1 << SPIE) | (1 << SPE) | (1 << MSTR) | (1 << CPHA) | (1 << CPOL);

    SPI_PORT &= ~(1 << SPI_CS);
    SPI_PORT |= (1 << SPI_CS);

    SPI_PORT &= ~(1 << SPI_SS);
    SPI_PORT |= (1 << SPI_SS);

    spiStatus = SPI_READY;

    sei();

    uint8_t buf[2] = {0};
    buf[0]         = 0x80 | 0x0F;

    SpiTxRx(buf, sizeof(buf));

    while (spiStatus != SPI_READY)
    {
        ;
    }
    PORTC = buf[1];

    while (1)
    {
        /* code */
    }
}

/*******************************************************************************
Функция обмена данными с ведомым устройством
Во время обмена устанавливается статус SPI_BUSY, после обмена устанавливается
статус SPI_READY

Аргументы:
pbuf        указатель на буфер для обмена
bufSize     размер буфера для обмена
*******************************************************************************/
void SpiTxRx(uint8_t* pbuf, uint8_t bufSize)
{
    // Если статус модуля не равен SPI_READY, то выходим из функции
    if (spiStatus != SPI_READY)
    {
        return;
    }

    // Установка статуса SPI_BUSY
    spiStatus = SPI_BUSY;

    // Сброс вывода СS (выбор ведомого устройства)
    SPI_PORT &= ~(1 << SPI_CS);

    // Установка размера и указателя на буфер обмена и
    // передача первого байта данных из буфера обмена
    bufSizeSpi = bufSize;
    pbufSpi    = pbuf;
    SPDR       = *pbufSpi;
}

/*******************************************************************************
Функция обработки события по завершению обмена одним байтом модуля SPI
Принятые данные помещаются в буфер обмена вместо переданных данных
*******************************************************************************/
void SpiTc(void)
{
    // Чтение пришедшего байт данных из SPDR и помещение его в буфер обмена на
    // место переданного байта
    *pbufSpi = SPDR;

    // Уменьшение количества байт обмена
    // Если оставшееся количество байт обмена ну равно нулю, то передаем
    // следующий байт данных из буфера обмена;
    // иначе, завершаем обмен: устанавливаем вывод CS и статус SPI_READY
    bufSizeSpi--;
    if (bufSizeSpi)
    {
        SPDR = *++pbufSpi;
    }
    else
    {
        SPI_PORT |= (1 << SPI_CS);
        spiStatus = SPI_READY;
    };
}
// End File --------------------------------------------------------------------