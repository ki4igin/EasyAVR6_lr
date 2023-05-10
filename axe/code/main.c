/**
 * Target:  Axe
 * Device:  ATmega16;
 * Board:   EasyAVR6;
 * Clock:   ext.clock 8 MHz
 * 
 * Программа считывает из микросхемы LSM6DSO32 идентификатор,
 * если считанный идентификатор верен, то он выводится на дисплей,
 * если считанный идентификатор неверен, то выводится на дисплей сообщение
 * об отсутствии устройства
 */

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "main.h"
#include "lsm.h"
#include "lcd16x2.h"
#include "converters.h"

/**
 * Функция чтения идентификатора LSM6DSO32
 * Функция блокирует выполнение основной программы, можно использовать только
 * при инициализации
 * 
 * \return идентификатор
 */
static uint8_t read_id(void)
{
    struct lsm_reg_pac pac = {
        .addr = LSM_WHO_AM_I
    };

    lsm_read_reg(&pac);

    while (spi_status != SPI_STATUS_RDY)
    {
        ;
    }

    return pac.reg.byte;
}

int main(void)
{
    spi_init();
    lcd_init();

    sei();

    uint8_t id = read_id();

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

    _delay_ms(2000);

    while (1)
    {        
    }
}

/* End File */