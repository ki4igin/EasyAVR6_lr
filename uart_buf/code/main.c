/**
 * Target:  Uart
 * Device:  ATmega16;
 * Board:   EasyAVR6;
 * Clock:   ext.clock 8 MHz
 * 
 * Программа осуществляет обмен по интерфейсу RS-232 с помощью модуля USART.
 * Основные параметры:
 *     скорость обмена 19200 бод, 
 *     8 бит данных,
 *     1 стоп-бит,
 *     бит паритета – нет.
 * Контроллер принимает пакет данных состоящий из 4-х байт:
 *      первые 3 байта – информационные; 
 *      последний – контрольная сумма.
 * Нажимая кнопку PB6 можно поочередно просмотреть принятые байты на семисегментных
 * индикаторах, которые подключены к портам B и C. На крайнем левом индикаторе
 * отображается номер просматриваемого байта, на двух правых -  сам принятый байт.
 * При нажатии на кнопку PB7 МК передает пакет данных состоящий из 3-х байт:
 *     1-й байт: 0х80,
 *     2-й байт: текущее состояния на выводах PA0...PA7,
 *     3-й байт: контрольная сумма.
 */
#include "avr/io.h"
#include "avr/interrupt.h"
#include <avr/pgmspace.h>

#include "main.h"
#include "sevseg.h"
#include "checksum.h"

// Variables -------------------------------------------------------------------
// uint8_t bufTx[NBUF_TX] = {0};   // Буфер передачи
// uint8_t bufRx[NBUF_RX] = {0};   // Буфер приема
// uint8_t dataRx[NBUF_RX] = {0};  // Массив принятых данных

struct user_flags flags = {0};  // Переменная пользовательских флагов

uint8_t rx_count = 0;
uint8_t rx_data[NBUF_RX] = {0};
uint8_t qqq_count = 0;
uint8_t qqq_data[NBUF_RX] = {0};
uint8_t tx_count = 0;
uint8_t tx_data[NBUF_TX] = {0};

int main(void)
{
    /**
     * PB6...PB7 - входы с PullUp (к ним подключены кнопки)
     * PA0...PA7 - входы с PullUp (к ним подключен контактный переключатель)
     */
    PORTB |= (1 << PB7) | (1 << PB6);
    PORTA = 0xFF;

    /**
     * Инициализация и включение таймера Т0
     * Режим: Normal; Предделитель: 64; TOP = 0xFF
     * Разрешение прерывания по переполнению
     * ОС0(PB3) не подключен
     * Время переполнения:
     * t = 256 * 64 / 8e6 = 2.048 мс, f = 8e6 / 64 / 256 = 488 Гц
     */
    TIMSK |= (1 << TOIE0);
    TCCR0 = (1 << CS01) | (1 << CS00);

    /**
     * Инициализация таймера Т1
     * Режим: CTC; Предделитель: 1; TOP = ОСR1A = 0x3E80
     * Разрешение прерывания от блока сравнения
     * ОС1A(PD5) не подключен
     * ОС1B(PD4) не подключен
     * Время переполнения t = ОСR1A / 8e6 = 2 мс
     */
    OCR1A = 0x3E80;
    TIMSK |= (1 << OCIE1A);
    TCCR1B = (1 << WGM12);

    /**
     * Инициализация и включение таймера Т2
     * Режим: Normal; Предделитель: 256; TOP = 0xFF
     * Разрешение прерывания по переполнению
     * ОС2(PD7) не подключен
     * Время переполнения:
     * t = 256 * 256 / 8e6 = 8.192 мс; f = 8e6 / 256 / 256 = 122 Гц
     */
    TIMSK |= (1 << TOIE2);
    TCCR2 = (1 << CS22) | (1 << CS21);

    /**
     * Инициализация UART, включение приемника и передатчика
     * Скорость обмена: 19200 бод, 8 бит данных, 1 стоп-бит, бит паритета: нет
     * Разрешение прерывания по приему
     * PD0(RXD) - вход с PullUp, PD1(TXD) - выход
     */
    PORTD |= (1 << PD0);

    uint16_t ubrr = F_CPU / (16UL * 19200) - 1;
    UBRRH = (uint8_t)(ubrr >> 8);
    UBRRL = (uint8_t)ubrr;
    UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
    UCSRB |= (1 << RXCIE) | (1 << RXEN) | (1 << TXEN);

    sevseg_init();

    sei();

    while (1)
    {
        if (flags.tx_req)
        {
            flags.tx_req = 0;

            // Формирование буфера данных для передачи
            // 1-й байт: 0х80,
            // 2-й байт: текущее состояния на выводах PA0...PA7,
            // 3-й байт: контрольная сумма.
            tx_count = 3;
            tx_data[0] = 0x80;
            tx_data[1] = PINA;
            tx_data[2] = check_sum_calc(tx_data, NBUF_TX - 1);

            // Запуск передачи (включение прерывания по опустошению РВВ UDR)
            UCSRB |= (1 << UDRIE);
        }
        if (flags.rx_complete)
        {
            flags.rx_complete = 0;

            // Вычисление контрольной суммы
            // Если последний байт принятых данных равен контрольной сумме, то
            // копируем данные из буфера приема bufRx в массив данных dataRx
            uint8_t check_sum = check_sum_calc(rx_data, NBUF_RX - 1);

            if (rx_data[NBUF_RX - 1] == check_sum)
            {
                for (uint8_t i = 0; i < NBUF_RX; i++)
                {
                    qqq_data[i] = rx_data[i];
                }
            }
        }
    }
}

/* End File */