#ifndef UART_H
#define UART_H

void uart_init(void);
void uart_send_byte(uint8_t byte);
void uart_send_word(uint16_t word);
void uart_send_array(const uint8_t *buf, uint8_t size);
void uart_send_str(uint8_t *str);
void _uart_send_strpgm(const char *str);

#define uart_send_strpgm(str) _uart_send_strpgm(PSTR(str))

#endif