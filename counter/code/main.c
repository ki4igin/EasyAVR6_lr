/**
 * Target:  Counter
 * Device:  ATmega16;
 * Board:   EasyAVR6;
 * Clock:   int.clock 8 MHz
 * 
 * Программа по нажатию кнопки "Вверх" (PA0) инкрементирует значение счетчика.
 * Счетчик изменяет свое значение от 0 до 9999.
 * Значение счетчика отображается на 4-х семисегментных индикаторах
 */

#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "main.h"
#include "sevseg.h"

struct user_flags
{
    uint8_t is_btn_lock : 1;
};

int main(void)
{
    struct user_flags flags = {0};

    uint16_t counter = {0};
    uint8_t digits[4] = {0};

    /* PA0...PA7 - входы c PullUp */
    DDRA = 0x00;
    PORTA = 0xFF;

    sevseg_init();

    while (1)
    {
        if ((PINA & (1 << PINA0)) == 0 && flags.is_btn_lock == 0)
        {
            flags.is_btn_lock = 1;
            counter = (counter < 9999) ? counter + 1 : 0;
        }
        else
        {
            flags.is_btn_lock = 0;
        }

        /* Задержка вывода на семисегментные индикаторы */
        _delay_us(1000);

        sevseg_bin2bcd(counter, digits);
        sevseg_display_process(digits);
    }
}

/* End File */