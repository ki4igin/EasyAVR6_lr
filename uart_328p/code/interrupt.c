#include <avr/io.h>
#include <avr/interrupt.h>

#include "main.h"
#include "sevseg.h"

/* Прерывание используется для обработки нажатий кнопок */
ISR(TIMER2_OVF_vect)
{
    if ((PINB & (1 << PINB4)) == 0)
    {
        if (flags.btn_lock == 0)
        {
            flags.btn_lock = 1;

            pac.num_byte++;
            if (pac.num_byte >= NBUF_RX)
            {
                pac.num_byte = 0;
            }
        }
    }
    else if ((PINB & (1 << PINB5)) == 0)
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
    rx.count = 0;
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
}

/**
 * Прерывание используется для отображения байта принятых данных на
 * семисегментных индикаторах
 */
ISR(TIMER0_OVF_vect)
{
    uint8_t temp = pac.data[pac.num_byte];

    uint8_t symbols[4] = {
        temp & 0x0F,
        temp >> 4,
        16,
        pac.num_byte
    };

    sevseg_display_process(symbols);
}

/* Прерывание используется приема пакета данных */
ISR(USART_RX_vect)
{
    /* Если принят первый байт, то начинаем формирование таймаута приема пакета */
    if (rx.count == 0)
    {
        TCNT1 = 0;
        TCCR1B |= (1 << CS10);
    }

    rx.data[rx.count++] = UDR0;

    /**
     * Если принят последний байт, то прекращаем формирование таймаута приема
     * пакета и устанавливаем флаг заверешения приема пакета данных
     */
    if (rx.count == NBUF_RX)
    {
        TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
        rx.count = 0;
        flags.rx_complete = 1;
    }
}

/* Прерывание используется для передачи пакета данных */
ISR(USART_UDRE_vect)
{
    UDR0 = tx.data[--tx.count];    
    
    if (tx.count == 0)
    {
        UCSR0B &= ~(1 << UDRIE0);
    }
}

/* End File */