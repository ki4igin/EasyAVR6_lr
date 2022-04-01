/**
 * Target:  Lcd
 * Device:  ATmega16;
 * Board:   EasyAVR6;
 * Clock:   ext.clock 8 MHz
 * 
 * Программа выводит на дисплей LCD16x2 приветственное сообщение "Hello World!"
 * По нажатию кнопки Cancel (PA5) экран дисплея включается и выключается
 */

#include "main.h"

int main(void)
{
    // Инициализация портов
    // PA5 - вход с PullUp (к нему подключена кнопка)
    PORTA |= (1 << PA5);

    // Инициализация и включение таймера Т2
    // Режим: Normal; Предделитель: 1024; TOP = 0xFF
    // Разрешение прерывания по переполнению
    // ОС2(PD7) не подключен
    // Время переполнения:
    // t = 1024 * 256 / 8e6 = 32.768 мс; f = 8e6 / 1024 / 256 = 31 Гц
    TIMSK |= (1 << TOIE2);
    TCCR2 = (1 << CS22) | (1 << CS21) | (1 << CS20);   

    lcd_init();

    lcd_disp_str((uint8_t*)"Hello World!");

    sei();

    while (1)
    {
    }
}

/* End File */