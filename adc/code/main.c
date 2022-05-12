/**
 * Target:  ADC
 * Device:  ATmega16;
 * Board:   EasyAVR6;
 * Clock:   ext.clock 8 MHz
 * 
 * Программа оцифровывает аналоговый сигнал на входе PA0 (нулевой канал ADC)
 * с разрядностью 8 бит с частотой 1 Гц.
 * Результат преобразования в виде шестнадцатеричного числа отправляется на ПК
 * интерфейсу UART и в виде десятичного числа отображается на LCD16x2 с
 * указанием номера канала АЦП.
 * 
 * Основные параметры UART:
 *      скорость обмена 19200 бод,
 *      8 бит данных,
 *      1 стоп-бит,
 *      бит паритета – нет.
 */
#include <avr/io.h>
#include <avr/interrupt.h>

#include "main.h"
#include "lcd16x2.h"
#include "converters.h"
#include "uart.h"

#define NBUF_LCD 5

#define PAC_HEAD 0x03
#define PAC_END  (~PAC_HEAD)

// uint8_t lcd_buf[NBUF_LCD] = {0};

volatile struct user_flags flags;
uint8_t adc_data;

int main(void)
{
    /**
     * Инициализация и включение таймера Т1
     * Режим: CTC; Предделитель: 256; TOP = ОСR1A = 0x7A12; OCR1B = 0x7A10
     * ОС1A(PD5), ОС1B(PD4) не подключены
     * Время переполнения:
     * t = 256 * 0x7A12 / 8e6 = 1 с; f = 8e6 / 256 / 0x7A12 = 1 Гц
     */
    OCR1B = 0x7A10;
    OCR1A = 0x7A12;
    TCCR1B = (1 << WGM12) | (1 << CS10);

    uart_init();

    /**
     * Инициализация и включение АЦП
     * Опорное напряжение: 2.56В; Выравнивание: по левому краю
     * Источник запуска: совпадение канала В таймера Т1
     * Режим: непрерывное преобразование; Предделитель: 64
     * Разрешение прерывания по завершению преобразования
     */
    ADMUX = (1 << REFS1) | (1 << REFS0) | (1 << ADLAR);
    SFIOR |= (1 << ADTS2) | (1 << ADTS0);
    ADCSRA = (1 << ADEN) | (1 << ADATE) | (1 << ADIE) |
             (1 << ADPS2) | (1 << ADPS1);

    lcd_init();
    lcd_disp_str("Ch0:");

    sei();

    while (1)
    {
        if (flags.adc_conv_complete)
        {
            flags.adc_conv_complete = 0;

            uint8_t buf[] = {
                PAC_HEAD,
                adc_data,
                0x00,
                PAC_END};

            uart_send_array(buf, sizeof(buf));

            static uint8_t cnt;
            if (cnt++ == 0)
            {
                static uint8_t lcd_buf[NBUF_LCD] = {0};
                uint2str(adc_data, lcd_buf);
                lcd_mov_cursor(6);
                lcd_disp_buf(lcd_buf, sizeof(lcd_buf));
            }
        }
    }
}

/* End File */