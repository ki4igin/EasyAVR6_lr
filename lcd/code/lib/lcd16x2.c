#include "lcd16x2.h"

#define LCD_PORT PORTD  // Порт к которому подключен LCD дисплей
#define LCD_DDR  DDRD   // Выводы данных LCD должны быть подключены к старшим
#define LCD_PIN  PIND   // выводам порта микроконтроллера

#define LCD_DATA_MASK 0xF0  // Маска выводов данных дисплея

#define LCD_E  3  // Номер вывода E (запись данных в LCD осуществляется по спаду)
#define LCD_RS 2  // Номер вывода RS (1 - данные; 0 - инструкция)

#define LCD_PIN_MASK (LCD_DATA_MASK | (1 << LCD_RS) | (1 << LCD_E))

/**
 * Функция отправки полубайта данных/команды на дисплей
 * Код полубайта должен находится в старшем полубайте аргумента hfbyte
 * 
 * \param hfbyte код полубайта данных/команды (старший полубайт)
 */
static void lcd_send_hfbyte(uint8_t hfbyte)
{
    LCD_PORT |= (1 << LCD_E);

    /* Выводы данных дисплея подключены к старшим разрядам порта МК */
    uint8_t temp = LCD_PORT;
    temp &= ~LCD_DATA_MASK;
    temp |= hfbyte;
    LCD_PORT = temp;

    LCD_PORT &= ~(1 << LCD_E);
}

/**
 * Функция отправки старшего полубайта команды на дисплей (используется при
 * инициализации). Код половины команды должен находится в старшем полубайте
 * аргумента hfcmd
 * 
 * \param hfcmd старший полубайт команды
 */
static void lcd_send_hfcmd(uint8_t hfcmd)
{
    LCD_PORT &= ~(1 << LCD_RS);
    lcd_send_hfbyte(hfcmd);
}

/**
 * Функция отправки байта данных/команды на дисплей
 *
 * \param byte байт данных/команды
 */
static void lcd_send_byte(uint8_t byte)
{
    lcd_send_hfbyte(byte & 0xF0);
    lcd_send_hfbyte(byte << 4);
}

/* Функция инициализации дисплея LCD16x2 */
void lcd_init(void)
{
    /* Инициализация выводов, подключенных к дисплею как выходов */
    LCD_DDR |= LCD_PIN_MASK;

    /* Задержка не менее 15 мс, после подачи питания на дисплей */
    _delay_ms(15);

    /**
     * Инициализация дисплея и перевод его на 4-х битный интерфейс
     * согласно порядку, приведенному в datasheet
     * Пока не перевели на 4-битный интерфейс отправляем команды по полбайта 
     * Отправка старшего полубайта команды настройки 8-битного интерфейса
     * Задержка не менее 4.1 мс
     * Отправка старшего полубайта команды настройки 8-битного интерфейса
     * Задержка не менее 100 мкс
     * Отправка старшего полубайта команды настройки 8-битного интерфейса
     * Задержка не менее 50 мкс
     * Отправка старшего полубайта команды настройки 4-битного интерфейса
     */
    lcd_send_hfcmd((1 << LCD_F) | (1 << LCD_F_DL));
    _delay_ms(5);
    lcd_send_hfcmd((1 << LCD_F) | (1 << LCD_F_DL));
    _delay_us(100);
    lcd_send_hfcmd((1 << LCD_F) | (1 << LCD_F_DL));
    _delay_us(50);
    lcd_send_hfcmd(1 << LCD_F);

    /**
     * Перевод дисплея на 4-битный интерфейс завершен. Теперь можно посылать
     * команды/данные по полному байту
     */

    /* Подтверждение 4-битного интерфейса */
    _delay_us(50);
    lcd_send_cmd(1 << LCD_F);

    /* Очистка дисплея, дисплей очищается не менее 1.5 мс */
    _delay_us(50);
    lcd_send_cmd(1 << LCD_CLR);
    _delay_ms(2);

    /* Включение экрана дисплея */
    _delay_us(50);
    lcd_send_cmd((1 << LCD_ON) | (1 << LCD_ON_D));
}

/**
 * Функция вывода на дисплей массива символов
 *
 * \param buf указатель на массив
 * \param size размер массива
 */
void lcd_disp_buf(uint8_t* buf, uint8_t size)
{
    do
    {
        _delay_us(50);
        lcd_send_char(*buf++);
    } while (--size);
}

/**
 * Функция вывода на дисплей строки
 * 
 * \param str указатель на строку
 */
void lcd_disp_str(uint8_t* str)
{
    do
    {
        _delay_us(50);
        lcd_send_char(*str);
    } while (*++str);
}

/**
 * Функция изменения позиции курсора дисплея
 * Время выполнения составляет 50 мкс
 * 
 * \param new_pos новая позиция курсора дисплея
 */
void lcd_mov_cursor(uint8_t new_pos)
{
    uint8_t temp = (1 << LCD_DDRAM) | new_pos;
    lcd_send_cmd(temp);
}

/**
 * Функция вывода на дисплей символа
 * Время выполнения составляет 50 мкс
 * 
 * \param symbol код символа
 */
void lcd_send_char(uint8_t symbol)
{
    LCD_PORT |= (1 << LCD_RS);
    lcd_send_byte(symbol);
}

/**
 * Функция отправки команды на дисплей
 * Время выполнения команд "Очистка дисплея с установкой курсора в начало первой
 * строки" и "Установка курсора в начало первой строки" составляет 1.5 мс;
 * время выполнения остальных команд составляет 50 мкс.
 * 
 * \param cmd код команды
 */
void lcd_send_cmd(uint8_t cmd)
{
    LCD_PORT &= ~(1 << LCD_RS);
    lcd_send_byte(cmd);
}

/* End File */