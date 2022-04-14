#ifndef LCD16X2_H
#define LCD16X2_H

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

/* Битовые поля для команд LCD дисплея */

/* Очистка дисплея с установкой курсора в начало первой строки */
#define LCD_CLR 0  // Старший бит инструкции (DB0)

/* Установка курсора в начало первой строки */
#define LCD_HOME 1  // Старший бит инструкции (DB1)

/* Установка направления вывода символов, разрешение сдвига экрана */
#define LCD_MODE     2  // Старший бит инструкции (DB2)
#define LCD_MODE_I_D 1  // Направление вывода символов (DB1) 
                        // 0: справа налево; 1: слева направо
#define LCD_MODE_SH  0  // Разрешение сдвига экрана при выводе символов (DB0)

/* Включение/выключение дисплея и отображения курсора */
#define LCD_ON   3  // Старший бит инструкции (DB3)
#define LCD_ON_D 2  // Включение дисплея (DB2)
#define LCD_ON_C 1  // Включение курсора (DB1)
#define LCD_ON_B 0  // Включение мигания курсора (DB0)

/* Сдвиг курсора и экрана */
#define LCD_SHIFT     4  // Старший бит инструкции (DB4)
#define LCD_SHIFT_S_C 3  // Разрешение сдвига экрана (DB3) 
                         // 0: сдвиг только курсора; 1: сдвиг курсора и экрана
#define LCD_SHIFT_R_L 2  // Направление сдвига (DB2) 
                         // 0: сдвиг влево; 1: сдвиг вправо

/* Настройка интерфейса, количества строк для вывода, размера шрифта */
#define LCD_F    5  // Старший бит инструкции (DB5)
#define LCD_F_DL 4  // Выбор интерфейса (DB4) 
                    // 0: 4-битный интерфейс; 1: 8-битный интерфейс
#define LCD_F_N  3  // Количество строк для вывода (DB3) 
                    // 0: 1 строка; 1: 2 строки
#define LCD_F_F  2  // Размер шрифта (DB2) 0: 5х8 пикселей; 1: 5х11 пикселей

/* Запись адреса DDRAM памяти в адресный указатель */
#define LCD_DDRAM 7  // Старший бит инструкции (DB7)

void lcd_init(void);
void lcd_disp_buf(uint8_t *buf, uint8_t size);
void lcd_disp_str(uint8_t *str);
void lcd_mov_cursor(uint8_t new_pos);
void lcd_send_cmd(uint8_t cmd);
void lcd_send_char(uint8_t data);

#define lcd_disp_strpgm(str)                         \
    do                                               \
    {                                                \
        const char *pstr = PSTR(str);                \
        do                                           \
        {                                            \
            _delay_us(50);                           \
            uint8_t symbol = pgm_read_byte(*pstr++); \
            if (symbol == 0)                         \
            {                                        \
                break;                               \
            }                                        \
            lcd_send_char(symbol);                   \
        }                                            \
        while (1);                                   \
    }                                                \
    while (0)

#endif

/* End File */