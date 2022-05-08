#include "avr/io.h"
#include "avr/interrupt.h"
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdio.h>

#include "main.h"
#include "converters.h"
#include "debug.h"

int main(void)
{
    /**
     * Инициализация UART, включение приемника и передатчика
     * Скорость обмена: 19200 бод, 8 бит данных, 1 стоп-бит, бит паритета: нет
     * Разрешение прерывания по приему
     * PD0(RXD) - вход с PullUp, PD1(TXD) - выход
     */
    uart_init();

    sei();

    uint8_t tmp;

    debug_send_message("Hello World\r\n");

    while (1)
    {
        _delay_ms(100);
        static uint8_t cnt;
        cnt++;
        debug_send_message("Counter = ");
        uint8_t cnt_str[3];
        uint8_t len = uint2str(cnt, cnt_str);
        debug_send_array(cnt_str, len);
        debug_send_message("\r\n");

        // error_handler();

        // _delay_ms(1000);

        // error_handler();

        // _delay_ms(1000);

        // error_handler();
    }
}

/* End File */