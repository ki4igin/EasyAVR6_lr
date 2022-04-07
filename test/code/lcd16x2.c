#include <util/delay.h>

#include "lcd16x2.h"

#define LCD_PORT PORTD  // Порт к которому подключен LCD дисплей
#define LCD_DDR  DDRD   // Выводы данных LCD должны быть подключены к старшим
#define LCD_PIN  PIND   // выводам порта микроконтроллера

#define LCD_DATA_MASK 0xF0  // Маска выводов данных дисплея

#define LCD_E  3  // Номер вывода E (запись данных в LCD осуществляется по спаду)
#define LCD_RS 2  // Номер вывода RS (1 - данные; 0 - инструкция)

#define LCD_PIN_MASK (LCD_DATA_MASK | (1 << LCD_RS) | (1 << LCD_E))

static void lcd_send_hfbyte(uint8_t hfbyte)
{
    LCD_PORT |= (1 << LCD_E);

    uint8_t temp = LCD_PORT;
    temp &= ~LCD_DATA_MASK;
    temp |= hfbyte;
    LCD_PORT = temp;

    LCD_PORT &= ~(1 << LCD_E);
}

static void lcd_send_hfcmd(uint8_t hfcmd)
{
    LCD_PORT &= ~(1 << LCD_RS);
    lcd_send_hfbyte(hfcmd);
}

static void lcd_send_byte(uint8_t byte)
{
    lcd_send_hfbyte(byte & 0xF0);
    lcd_send_hfbyte(byte << 4);
}

void lcd_init(void)
{
    LCD_DDR |= LCD_PIN_MASK;

    _delay_ms(15);

    lcd_send_hfcmd((1 << LCD_F) | (1 << LCD_F_DL));
    _delay_ms(5);
    lcd_send_hfcmd((1 << LCD_F) | (1 << LCD_F_DL));
    _delay_us(100);
    lcd_send_hfcmd((1 << LCD_F) | (1 << LCD_F_DL));
    _delay_us(50);
    lcd_send_hfcmd(1 << LCD_F);

    _delay_us(50);
    lcd_send_cmd(1 << LCD_F);

    _delay_us(50);
    lcd_send_cmd(1 << LCD_CLR);
    _delay_ms(2);

    _delay_us(50);
    lcd_send_cmd((1 << LCD_ON) | (1 << LCD_ON_D));
}

void lcd_disp_buf(uint8_t *buf, uint8_t size)
{
    do
    {
        _delay_us(50);
        lcd_send_char(*buf++);
    }
    while (--size);
}

void lcd_disp_str(uint8_t *str)
{
    do
    {
        _delay_us(50);
        lcd_send_char(*str);
    }
    while (*++str);
}

void lcd_mov_cursor(uint8_t new_pos)
{
    uint8_t temp = (1 << LCD_DDRAM) | new_pos;
    lcd_send_cmd(temp);
}

void lcd_send_char(uint8_t symbol)
{
    LCD_PORT |= (1 << LCD_RS);
    lcd_send_byte(symbol);
}

void lcd_send_cmd(uint8_t cmd)
{
    LCD_PORT &= ~(1 << LCD_RS);
    lcd_send_byte(cmd);
}

/* End File */