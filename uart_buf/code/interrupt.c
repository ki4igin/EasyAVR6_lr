#include <avr/io.h>
#include <avr/interrupt.h>

#include "main.h"
#include "sevseg.h"

/* Прерывание используется для обработки нажатий кнопок */
ISR(TIMER2_OVF_vect)
{
    if ((PINB & (1 << PINB6)) == 0)
    {
        if (flags.btn_lock == 0)
        {
            flags.btn_lock = 1;

            qqq_count++;
            if (qqq_count >= NBUF_RX)
            {
                qqq_count = 0;
            }
        }
    }
    else if ((PINB & (1 << PINB7)) == 0)
    {
        if (flags.btn_lock == 0)
        {
            flags.btn_lock = 1;
            flags.tx_req = 1;
        }
    }
    else
    {
        flags.btn_lock = 0;
    }
}

/**
 * Прерывание используется для формирования таймаута приема данных,
 * для обеспечения целостности посылки
 */
ISR(TIMER1_COMPA_vect)
{
    rx_count = 0;
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
}

/**
 * Прерывание используется для отображения байта принятых данных на
 * семисегментных индикаторах
 */
ISR(TIMER0_OVF_vect)
{
    uint8_t temp = qqq_data[qqq_count];

    uint8_t buf[4] = {
        temp & 0x0F,
        temp >> 4,
        16,
        qqq_count
    };

    sevseg_display_process(buf);
}

/* Прерывание используется приема пакета данных */
ISR(USART_RXC_vect)
{
    /* Если принят первый байт, то начинаем формирование таймаута приема пакета */
    if (rx_count == 0)
    {
        TCNT1 = 0;
        TCCR1B |= (1 << CS10);
    }

    rx_data[rx_count] = UDR;
    rx_count++;

    /**
     * Если принят последний байт, то прекращаем формирование таймаута приема
     * пакета и устанавливаем флаг заверешения приема пакета данных
     */
    if (rx_count == NBUF_RX)
    {
        TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
        rx_count = 0;
        flags.rx_complete = 1;
    }
}

/* Прерывание используется для передачи пакета данных */
ISR(USART_UDRE_vect)
{
    UDR = tx_data[tx_count];
    tx_count--;

    if (tx_count == 0)
    {
        UCSRB &= ~(1 << UDRIE);
    }
}

/* End File */