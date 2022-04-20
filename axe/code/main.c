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
#include <avr/pgmspace.h>
#include <assert.h>
#include <util/delay.h>

#include "main.h"
#include "lsm.h"
#include "lcd16x2.h"
#include "converters.h"

// Functions -------------------------------------------------------------------
int main(void)
{
    spi_init();
    lcd_init();

    sei();

    struct lsm_reg_pac pac = {
        .addr = LSM_WHO_AM_I};

    lsm_read_reg(&pac);

    while (spi_status != SPI_STATUS_READY)
    {
        ;
    }

    uint8_t id = pac.reg.byte;

    if (id == LSM_ID)
    {
        lcd_disp_str("LSM6D ID: ");
        uint8_t id_str[] = {
            (id >> 4) + '0',
            (id & 0x0F) + '0',
        };
        lcd_disp_buf(id_str, sizeof(id_str));
    }
    else
    {
        lcd_disp_str("LSM6D not found");
        while (1)
        {
        }
    }

    struct lsm_reg_pac xl = {
        .addr = LSM_CTRL1_XL,
        .reg.ctrl1_xl = {
            .xl_fs = LSM_XL_FS_4g,
            .xl_odr = LSM_XL_ODR_12Hz5}};

    spi_txrx((uint8_t *)&xl, sizeof(xl));

    _delay_ms(2000);

    // Основной цикл
    while (1)
    {
        struct lsm_word_pac temp_pac = {
            .addr = LSM_OUTZ_L_A};

        lsm_read_word(&temp_pac);
        _delay_ms(100);

        struct lsm_word temp_word = temp_pac.data;

        int16_t temp = (int16_t)(temp_word.h * 256 + temp_word.l) * 4 * 1000L / (1U << 15);

        uint8_t temp_str[7];
        int2str(temp, 2, temp_str, sizeof(temp_str));
        lcd_mov_cursor(5);
        lcd_disp_buf(temp_str, sizeof(temp_str));
        _delay_ms(100);
    }
}

/* End File */