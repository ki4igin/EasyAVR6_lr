#include <avr/pgmspace.h>
#include "sevseg.h"

#define DATA_PORT PORTC  // Порт данных к которому подключены
#define DATA_DDR  DDRC   // семисегментные индикаторы.

#define CTRL_PORT PORTB  // Управляющий порт к которому подключены общие
#define CTRL_DDR  DDRB   // катоды/аноды семисегментных индикаторов
#define CTRL_PIN3 3      // Номер вывода общего а/к четвертого индикатора
#define CTRL_PIN2 2      // Номер вывода общего а/к третьего индикатора
#define CTRL_PIN1 1      // Номер вывода общего а/к второго индикатора
#define CTRL_PIN0 0      // Номер вывода общего а/к четвертого индикатора

#define CTRL_PIN_MASK ((1 << CTRL_PIN3) | (1 << CTRL_PIN2) | \
                       (1 << CTRL_PIN1) | (1 << CTRL_PIN0))

const uint8_t ctrl_pins[4] PROGMEM = {
    (1 << CTRL_PIN0),
    (1 << CTRL_PIN1),
    (1 << CTRL_PIN2),
    (1 << CTRL_PIN3),
};

/* Таблица перекодировки символов в памяти программ */
const uint8_t dec2sevseg[10] PROGMEM = {
    0b11111100,  // код "0"
    0b01100000,  // код "1"
    0b11011010,  // код "2"
    0b11110010,  // код "3"
    0b01100110,  // код "4"
    0b10110110,  // код "5"
    0b10111110,  // код "6"
    0b11100000,  // код "7"
    0b11111110,  // код "8"
    0b11110110   // код "9"
};

/**
 * Функция инициализации 4-х семисегментных индикаторов
 * Выводы данных подключены к DATA_PORT
 * Выводы управления подключены к CTRL_PORT
 */
void sevseg_init(void)
{
    /**
     * Инициализация выводов подключенных к индикаторам и 
     * включение первого индикатора
     */
    DATA_DDR = 0xFF;
    DATA_PORT = 0x00;
    CTRL_DDR |= CTRL_PIN_MASK;
    CTRL_PORT &= ~CTRL_PIN_MASK;

    // TODO проверить нужна ли данная строчка
    CTRL_PORT |= (1 << CTRL_PIN0);
}

/**
 * Функция перевода 2-х байтного числа в 4-х разрядное двоично-десятичное число.
 * 
 * \param data  исходное 2-х байтное число
 * \param digs  указатель на 4-х байтный буфер, в который записываются разряды
 *              двоично-десятичного числа.
 */
void sevseg_bin2bcd(uint16_t data, uint8_t *digs)
{
    /* Установка указателя на последний (страший) разряд */
    digs += 4 - 1;

    /* Вычисление разрядов числа, начиная со старшего */
    *digs-- = data / 1000;
    data %= 1000;
    *digs-- = data / 100;
    data %= 100;
    *digs-- = data / 10;
    data %= 10;
    *digs-- = data / 1;
    data %= 1;  // ?
}

/**
 * Функция отображения следующего разряда 4-х разрядного двоично-десятичного
 * числа на 4-х семисегментных индикаторах. Для отображения всего числа, функцию
 * необходимо переодически вызывать.
 * 
 * \param digs  указатель на 4-х байтное двоично-десятичное число
 */
void sevseg_disp(uint8_t *digs)
{
    /* Выключение отображения на семисегментных индикаторах */
    DATA_PORT = 0x00;
    CTRL_PORT &= ~CTRL_PIN_MASK;

    /* Внутренний счетчик по модулю 4, для перебора разрядов числа */
    register uint8_t cnt_dig asm("r4");
    cnt_dig = (cnt_dig + 1) & 0x03;

    /**
     * Включение индикатора с номером cnt_dig и вывод в порт данных
     * соответствующего разряда двоично-десятичного числа, используя таблицу
     * перекодировки символов dec2sevseg в памяти программ
     */
    CTRL_PORT |= pgm_read_byte(&ctrl_pins[cnt_dig]);
    uint8_t dig = digs[cnt_dig];
    uint8_t symb = pgm_read_byte(&dec2sevseg[dig]);
    DATA_PORT = symb;
}

/**
 * Функция выключения семисегментных индикаторов
 */
void sevseg_off(void)
{
    DATA_PORT = 0x00;
}

/* End File */