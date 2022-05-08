#ifndef DEBUG_H
#define DEBUG_H

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "uart.h"
#include "converters.h"



#ifdef DEBUG
#define error_handler() _error_handler(PSTR(__FILE__), __LINE__)

#define debug_send_message(str) uart_send_strpgm(str)

void debug_send_array(void *buf, int size)
{
    uart_send_array((uint8_t *)buf, size);
}

void debug_send_word(int data)
{
    uart_send_word(data);
}
#else
#define error_handler() 

void debug_send_message(char *str)
{
    (void)str;
}
void debug_send_array(void *pbuf, int len)
{
    (void)pbuf;
    (void)len;
}
void debug_send_word(int data)
{
    (void)data;
}
#endif

void _error_handler(const char *file, int line)
{
    debug_send_message("Error in file ");
    _uart_send_strpgm(file);
    debug_send_message("; in line ");

    uint8_t line_str[5];
    uint8_t len = uint2str(line, line_str);
    debug_send_array(line_str, len);
    debug_send_message("\r\n");

    // while (1)
    // {
    // }
}

#endif
