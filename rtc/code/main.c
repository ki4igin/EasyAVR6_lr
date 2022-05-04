/*******************************************************************************
Target:  RTC
Device:  ATmega16;
Board:   EasyAVR6;
Clock:   ext.clock 8 MHz

Программа считывает текущее время (минуты и секунды) из микросхемы часов
реального времени PCF8583 и отображает его на дисплее LCD16x2 в формате "mm:ss".
Обмен между микроконтроллером и часами реального времени осуществляется по
интерфейсу i2c с частотой 100 кГц.
*******************************************************************************/

#include <avr/interrupt.h>

#include "main.h"
#include "twi.h"
#include "pcf8583.h"
#include "lcd16x2.h"

struct pcf_time time = {0};

/* Буфер для вывода времени на дисплей, формат вывода "mm:ss" */
uint8_t lcd_buf[3 * sizeof(time) - 1] = {0};

/**
 * Функция преобразования структуры времени в строку в формате "mm:ss"
 * 
 * \param time указатель на структуру времени
 * \param buf указатель на буфер результирующией строки
 */
static inline void time2str(struct pcf_time *time, uint8_t *buf)
{
    *buf++ = (time->minutes >> 4) + '0';
    *buf++ = (time->minutes & 0x0F) + '0';
    *buf++ = ':';
    *buf++ = (time->seconds >> 4) + '0';
    *buf++ = (time->seconds & 0x0F) + '0';
}

int main(void)
{
    /**
     * Инициализация и включение таймера Т2
     * Режим: Normal; Предделитель: 1024; TOP = 0xFF
     * ОС2(PD7) не подключен
     * Время переполнения:
     * t = 256 * 1024 / 8e6 = 32.768 мс; f = 8e6 / 1024 / 256 = 30 Гц
     */
    TCCR2 = (1 << CS22) | (1 << CS21) | (1 << CS20);

    lcd_init();
    lcd_disp_str("Time:");

    twi_init();
    sei();
    pcf_init();

    while (1)
    {
        if (TIFR & (1 << TOV2))
        {
            TIFR |= (1 << TOV2);
            pcf_read_time(&time);
        }

        if (twi_status == TWI_RX_COMPLETE)
        {
            twi_status = TWI_READY;

            time2str(&time, lcd_buf);
            lcd_mov_cursor(6);
            lcd_disp_buf(lcd_buf, sizeof(lcd_buf) / sizeof(lcd_buf[0]));
        }
    }
}

/* End File */