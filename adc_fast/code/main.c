/**
 * Target:  ADC
 * Device:  ATmega16;
 * Board:   EasyAVR6;
 * Clock:   ext.clock 8 MHz
 * 
 * Программа оцифровывает аналоговый сигнал на входе PA0 (нулевой канал ADC)
 * с разрядностью 8 бит с частотой 10 Гц.
 * Результат преобразования отправляется на ПК интерфейсу UART и отображается в
 * виде десятичного числа отображается на LCD16x2 с указанием номера канала АЦП.
 * 
 * Параметры UART:
 *      скорость обмена 19200 бод,
 *      8 бит данных,
 *      1 стоп-бит,
 *      бит паритета – нет.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "main.h"
#include "lcd16x2.h"
#include "converters.h"
#include "uart.h"

struct uart_pac
{
    const uint8_t head;
    uint16_t adc0;
    const uint8_t end;
} uart_pac = {
    .head = 0x03,
    .end = ~0x03};

volatile struct user_flags flags;
uint16_t adc_data;

int main(void)
{
    uart_init();

    SFIOR |= (1 << ACME);
    ACSR |= (1 << ACIE) | (1 << ACIS1) | (1 << ACIS0);

    /**
     * Инициализация и включение АЦП
     * Опорное напряжение: 2.56В; Выравнивание: по левому краю
     * Источник запуска: совпадение канала В таймера Т1
     * Режим: непрерывное преобразование; Предделитель: 64
     * Разрешение прерывания по завершению преобразования
     */
    ADMUX = (1 << REFS0) | (1 << ADLAR);
    SFIOR |= (1 << ADTS0);
    ADCSRA = (1 << ADEN) | (1 << ADATE) | (1 << ADIE) |
             (1 << ADPS0);

    lcd_init();
    lcd_disp_str("Ch0:");

    sei();
    
    // uart_send_byte(0x03);

    uint8_t cnt = 0;

    while (1)
    {
        if (flags.adc_data_rdy)
        {
            flags.adc_data_rdy = 0;

            if (cnt == 0)
            {
                uart_send_byte(0x03);
            }
            uart_send_word(adc_data);
            if (cnt == 0xFF)
            {
                uart_send_byte(~0x03);
                SFIOR |= (1 << ADTS0);

                _delay_ms(1000);

                ACSR &= ~(1 << ACD);
                ACSR |= (1 << ACIE);
            }

            cnt++;
        }
    }
}

/* End File */