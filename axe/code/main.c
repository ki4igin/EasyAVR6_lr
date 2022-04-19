// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

/*******************************************************************************
Target:  Counter2.0
Device:  ATmega16;
Board:   EasyAVR6;
Clock:   ext.clock 8 MHz

Программа при нажатии кнопки "Вверх" (PA0) увеличивает значение счетчика
от 0000 до 9999. Текущее значение счетчика отображается на семисегментных
индикаторах. Семисегментные индикаторы подключены к расширителю портов.
Обмен между расширителем портов и МК осуществляется по интерфейсу SPI.
*******************************************************************************/

// Includes --------------------------------------------------------------------
#include "main.h"
#include "lsm6dso32.h"
#include "lcd16x2.h"
#include "assert.h"
#include "util/delay.h"

static inline void num2dig(uint16_t value, uint8_t *digits)
{
    /* Установка указателя на последний (старший) разряд */

    /* Вычисление разрядов числа, начиная со старшего */
    *digits++ = value / 10000 + '0';
    value %= 10000;
    *digits++ = value / 1000 + '0';
    value %= 1000;
    *digits++ = value / 100 + '0';
    value %= 100;
    *digits++ = value / 10 + '0';
    value %= 10;
    *digits++ = value / 1 + '0';
    value %= 1;  // ?
}

// Functions -------------------------------------------------------------------
int main(void)
{
    // Инициализация портов
    // PA0...PA7 - входы с PullUp (к ним подключены кнопки)
    // PB2 - выход с уровнем лог. 1 (к нему подключен RESET расширителя портов)
    DDRA = 0xFF;
    PORTA = 0x00;

    DDRC = 0xFF;
    DDRB |= (1 << PB2);
    PORTB |= (1 << PB2);

    // Инициализация и включение таймера Т0
    // Режим: Normal, Предделитель: 64; TOP = 0xFF
    // Разрешение прерывания по переполнению
    // ОС2(PD7) не подключен
    // Время переполнения:
    // t = 256 * 64 / 8e6 = мс; f = 8e6 / 64 / 256 = 488 Гц
    // TIMSK |= (1 << TOIE0);
    TCCR0 = (1 << CS01) | (1 << CS00);

    // Инициализация и включение таймера Т2
    // Режим: Normal; Предделитель: 1024; TOP = 0xFF
    // Разрешение прерывания по переполнению
    // ОС2(PD7) не подключен
    // Время переполнения:
    // t = 1024 * 256 / 8e6 = 32.768 мс; f = 8e6 / 1024 / 256 = 31 Гц
    // TIMSK |= (1 << TOIE2);
    TCCR2 = (1 << CS22) | (1 << CS21) | (1 << CS20);

    // Инициализация модуля SPI
    spi_init();

    lcd_init();

    // Разрешение глобальных прерываний
    sei();

    // struct ucf_line temp =
    // {
    //     .address = LSM6DSO32_CTRL1_XL,
    //     .data.ctrl1_xl = {
    //         .fs_xl = LSM6DSO32_FS_XL_4g,
    //         .odr_xl = LSM6DSO32_XL_ODR_52Hz}
    // };

    struct ctrl_line id = {
        .address = LSM6DSO32_WHO_AM_I | 0x80,
        .reg = {
            .byte = 0,
            
        }
    };

    // static_assert(sizeof(id) == 1, "EEEEEEEE");

    spi_txrx((uint8_t *)&id, 2);

    while (spi_status != SPI_STATUS_READY)
    {
        ;
    }

    if (id.reg.byte == LSM6DSO32_ID)
    {
        lcd_disp_str((uint8_t *)"LSM6D ID: ");
        uint8_t id_str[] = {
            (id.reg.byte >> 4) + '0',
            (id.reg.byte & 0x0F) + '0',
        };
        lcd_disp_buf(id_str, sizeof(id_str));
    }
    else
    {
        lcd_disp_str((uint8_t *)"LSM6D not found");
    }

    // struct ucf_line xl =
    //     {
    //         .address = LSM6DSO32_CTRL1_XL,
    //         .reg.ctrl1_xl = {
    //             .fs_xl = LSM6DSO32_FS_XL_4g,
    //             .odr_xl = LSM6DSO32_XL_ODR_52Hz}};

    // spi_txrx((uint8_t *)&xl, sizeof(xl));

    // while (spi_status != SPI_STATUS_READY)
    // {
    //     ;
    // }

    // uint8_t tmp_buf[3] = {0};

    // Основной цикл
    while (1)
    {
        // tmp_buf[0] = LSM6DSO32_OUT_TEMP_L | 0x80;
        // spi_txrx((uint8_t *)&tmp_buf, sizeof(tmp_buf));
        // _delay_ms(100);
        // uint16_t temp = (uint32_t)(tmp_buf[2] * 256 + tmp_buf[1]) * 1000 / 256 + 25000;

        // uint8_t temp_dig[5];
        // num2dig(temp, temp_dig);
        // lcd_mov_cursor(10);
        // lcd_disp_buf(temp_dig, 5);
        // _delay_ms(100);
    }
}
// End File --------------------------------------------------------------------