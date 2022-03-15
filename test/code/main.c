#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "timer.h"

#define DEF_REG_BITS(name, addr, bit7, bit6, bit5, bit4, bit3, bit2, bit1, bit0) \
    volatile union                                                               \
    {                                                                            \
        uint8_t ALL;                                                             \
        struct                                                                   \
        {                                                                        \
            uint8_t bit0 : 1;                                                    \
            uint8_t bit1 : 1;                                                    \
            uint8_t bit2 : 1;                                                    \
            uint8_t bit3 : 1;                                                    \
            uint8_t bit4 : 1;                                                    \
            uint8_t bit5 : 1;                                                    \
            uint8_t bit6 : 1;                                                    \
            uint8_t bit7 : 1;                                                    \
        };                                                                       \
    } name __attribute__((io(addr)));

DEF_REG_BITS(PORTA_BITS, &PORTA, P7, P6, P5, P4, P3, P2, P1, P0)

#define concat(a, b)    a##b
#define macro_var(name) concat(name, __LINE__)

#define defer(start, end) for (      \
    int macro_var(_i_) = (start, 0); \
    !macro_var(_i_);                 \
    (macro_var(_i_) += 1), end)

// #define PORTA_BITS ((volatile struct PORTA_bits *)&PORTA)

void begin(void)
{
    PORTA = 0xFF;
}

void end(void)
{
    PORTA = 0x00;
}

int main(void)
{
    // Основной цикл

    defer(begin(), end())
    {
        PORTA_BITS.P0 = 0;
        PORTA_BITS.ALL = 5;
    }

    timer_init(0xFA);

    while (1)
    {
        timer_inc();
        PORTA_BITS.P0 = 0;
        PORTA_BITS.P0 = 1;
    }
}

__attribute__((weak)) void timer_comp(void)
{
    PORTD = ~PORTD;
}

/* End File */