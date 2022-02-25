/**
 * Target:  Counter
 * Device:  ATmega16;
 * Board:   EasyAVR6;
 * Clock:   int.clock 8 MHz
 * 
 * Программа по нажатию кнопки Up (PA0) увеличивает значение счетчика на 1.
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
    uint8_t btn_on : 1;
};

int main(void)
{
    struct user_flags flags = {0};

    uint16_t cnt = {0};
    uint8_t digs[4] = {0};

    /**
     * Инициализация портов 
     * PA0...PA7 - входы c PullUp
     */
    DDRA = 0x00;
    PORTA = 0xFF;

    sevseg_init();

    while (1)
    {
        /**
         * Если PA0 нажата (активный уровень низкий) и флаг btn_on сброшен,
         * то инкрементируем счетчик,
         * иначе сбрасываем флаг btn_on
         */
        if ((PINA & (1 << PINA0)) == 0 && flags.btn_on == 0)
        {
            flags.btn_on = 1;
            cnt = (cnt < 9999) ? cnt + 1 : 0;
        }
        else
        {
            flags.btn_on = 0;
        }

        /* Задержка вывода на семисегментные индикаторы */
        _delay_us(1000);

        /**
         * Преобразование значения счетчика в 4-х разрядное двоично-десятичное
         * число (размерность буфера для числа должна строго равняться 4)
         */
        sevseg_bin2bcd(cnt, digs);

        /**
         * Отображение на 4-х семисегментных индикаторах 4-х разрядного числа
         * (размерность буфера для числа должна строго равняться 4)
         */
        sevseg_disp(digs);
    }
}

/* End File */