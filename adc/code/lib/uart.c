#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "fifo.h"
#include "uart.h"

fifo_declarate(uart_fifo, 32);

void uart_init()
{
    PORTD |= (1 << PD0);

    uint16_t ubrr = F_CPU / (16UL * 19200) - 1;
    UBRRH = (uint8_t)(ubrr >> 8);
    UBRRL = (uint8_t)ubrr;
    UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
    UCSRB |= (1 << RXCIE) | (1 << RXEN) | (1 << TXEN);
}

void uart_send_byte(uint8_t byte)
{
    fifo_push(&uart_fifo, byte);
    UCSRB |= (1 << UDRIE);
}

void uart_send_word(uint16_t word)
{
    fifo_push(&uart_fifo, word >> 8);
    fifo_push(&uart_fifo, (uint8_t)word);
    UCSRB |= (1 << UDRIE);
}

void uart_send_array(const uint8_t *buf, uint8_t size)
{
    do
    {
        fifo_push(&uart_fifo, *buf++);
    } while (--size);    
    UCSRB |= (1 << UDRIE);
}

void uart_send_str(uint8_t *str)
{
    do
    {
        fifo_push(&uart_fifo, *str);
    }
    while (*++str);
    UCSRB |= (1 << UDRIE);
}

void _uart_send_strpgm(const char *str)
{
    do
    {
        uint8_t symbol = pgm_read_byte(str++);
        if (symbol == 0)
        {
            break;
        }
        fifo_push(&uart_fifo, symbol);
    }
    while (1);
    UCSRB |= (1 << UDRIE);
}

/* Прерывание используется для передачи пакета данных */
ISR(USART_UDRE_vect)
{
    UDR = fifo_pop(&uart_fifo);

    if (fifo_is_empty(&uart_fifo))
    {
        UCSRB &= ~(1 << UDRIE);
    }
}