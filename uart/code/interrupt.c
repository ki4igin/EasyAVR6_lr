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

            if (++disp_num_byte == NBUF_RX)
            {
                disp_num_byte = 0;
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
    buf_rx.count = 0;
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
}

/**
 * Прерывание используется для отображения байта принятых данных на
 * семисегментных индикаторах
 */
ISR(TIMER0_OVF_vect)
{
    uint8_t temp = rx_data[disp_num_byte];

    uint8_t symbols[4] = {
        temp & 0x0F,
        temp >> 4,
        16,
        disp_num_byte
    };

    sevseg_display_process(symbols);
}

/* Прерывание используется приема пакета данных */
ISR(USART_RXC_vect)
{
    /* Если принят первый байт, то начинаем формирование таймаута приема пакета */
    if (buf_rx.count == 0)
    {
        TCNT1 = 0;
        TCCR1B |= (1 << CS10);
    }

    buf_rx.data[buf_rx.count++] = UDR;

    /**
     * Если принят последний байт, то прекращаем формирование таймаута приема
     * пакета и устанавливаем флаг завершения приема пакета данных
     */
    if (buf_rx.count == NBUF_RX)
    {
        TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
        buf_rx.count = 0;
        flags.rx_complete = 1;
    }
}

/* Прерывание используется для передачи пакета данных */
ISR(USART_UDRE_vect)
{
    UDR = buf_tx.data[NBUF_TX - buf_tx.count];    
    
    if (--buf_tx.count == 0)
    {
        UCSRB &= ~(1 << UDRIE);
    }
}

/* End File */