/*******************************************************************************
Target:  Counter
Device:  ATmega16;
Board:   EasyAVR6;
Clock:   ext.clock 8 MHz

Программа по нажатию кнопки Up (PA0) увеличивает значение счетчика на 1.
Счетчик изменяет свое значение от 0 до 9999.
Значение счетчика отображается на 4-х семисегментных индикаторах
*******************************************************************************/

// Includes --------------------------------------------------------------------
#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>

// Typedefs --------------------------------------------------------------------
// Тип пользовательских флагов
typedef struct
{
    uint8_t btnOn : 1;  // Флаг нажатия кнопки
} Flags_t;

// Functions -------------------------------------------------------------------
// static inline uint8_t is_btn_on(void) {
//     return SREG & (1 << SREG_T);
// }

// typedef struct
// {
//     uint8_t C : 1;
//     uint8_t Z : 1;
//     uint8_t N : 1;
//     uint8_t V : 1;
//     uint8_t S : 1;
//     uint8_t H : 1;
//     uint8_t T : 1;
//     uint8_t I : 1;
// } flags_t;

typedef union
{
    struct
    {
        uint8_t C : 1;
        uint8_t Z : 1;
        uint8_t N : 1;
        uint8_t V : 1;
        uint8_t S : 1;
        uint8_t H : 1;
        uint8_t T : 1;
        uint8_t I : 1;
    };
    uint8_t byte;
} flags_t;

#define SREG_BITS ((flags_t *)&SREG)



int main(void)
{
    Flags_t flag = {0};    // Переменная пользовательских флагов
    uint16_t count = {0};  // 16-битный счетчик
    uint8_t buf[4] = {0};  // Буфер для хранения 4-х разрядного числа для
                           // вывода на семисегментные индикаторы

    // Инициализация портов
    // PA0...PA7 - входы c PullUp
    DDRA = 0x00;
    PORTA = 0xFF;

    SREG_BITS->T = 1;
    SREG_BITS->byte = 0b1111;

    // Основной цикл
    while (1)
    {
        
    }
}
// End File --------------------------------------------------------------------