#include <avr/io.h>
#include <avr/interrupt.h>

#include "main.h"
#include "sevseg.h"

uint8_t num_byte = {0};
uint8_t cnt_rx_bytes = {0};

/* Прерывание используется для обработки нажатий кнопок */
ISR(TIMER2_OVF_vect)
{
    if ((PINB & (1 << PINB6)) == 0)
    {
        if (flags.is_btn_lock == 0)
        {
            flags.is_btn_lock = 1;

            num_byte++;
            if (num_byte >= NBUF_RX)
            {
                num_byte = 0;
            }
        }
    }
    else if ((PINB & (1 << PINB7)) == 0)
    {
        if (flags.is_btn_lock == 0)
        {
            flags.is_btn_lock = 1;
            flags.tx = 1;
        }
    }
    else
    {
        flags.is_btn_lock = 0;
    }
}

/**
 * Прерывание используется для формирования таймаута приема данных,
 * для обеспечения целостности посылки
 */
ISR(TIMER1_COMPA_vect)
{
    cnt_rx_bytes = 0;
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
}

/**
 * Прерывание используется для отображения байта принятых данных на
 * семисегментных индикаторах
 */
ISR(TIMER0_OVF_vect)
{
    uint8_t buf[4];
    uint8_t *pbuf = buf;

    uint8_t temp = dataRx[num_byte];

    *pbuf++ = temp & 0x0F;
    *pbuf++ = temp >> 4;
    *pbuf++ = 16;
    *pbuf++ = num_byte;

    sevseg_display_process(buf);
}

/* Прерывание используется приема пакета данных */
ISR(USART_RXC_vect)
{
    /* Если принят первый байт, то начинаем формирование таймаута приема пакета */
    if (cnt_rx_bytes == 0)
    {
        TCNT1 = 0;
        TCCR1B |= (1 << CS10);
    }

    bufRx[cnt_rx_bytes] = UDR;
    cnt_rx_bytes++;

    /**
     * Если принят последний байт, то прекращаем формирование таймаута приема
     * пакета и устанавливаем флаг заверешения приема пакета данных
     */
    if (cnt_rx_bytes == NBUF_RX)
    {
        TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
        cnt_rx_bytes = 0;
        flags.rx = 1;
    }
}

/* Прерывание используется для передачи пакета данных */
ISR(USART_UDRE_vect)
{
    static uint8_t cntTx;

    UDR = bufTx[cntTx];
    cntTx++;
    if (cntTx == NBUF_TX)
    {
        UCSRB &= ~(1 << UDRIE);
        cntTx = 0;
    }
}

/* End File */