#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <assert.h>

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
#define to_string(a)    #a
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

struct reg
{
    uint8_t port;
    uint8_t ddr;
    uint8_t pin;
};

#define cstr(str) str[sizeof(str)]

#define add_line(message) concat(name, __LINE__)
#define get_str(message)  #message
// uint8_t s[] = cstr;

#define COLORS \
    X(RED)     \
    X(BLACK)   \
    X(WHITE)   \
    X(BLUE)

// Creating an enum of colors
// by macro expansion.
enum colors
{
#define X(value) value,
    X(RED) X(BLACK) X(WHITE) X(BLUE)
#undef X
};

#define WARN_IF(EXP)                                \
    do                                              \
    {                                               \
        if (EXP)                                    \
            fprintf(stderr, "Warning: " #EXP "\n"); \
    }                                               \
    while (0)

static_assert(sizeof(get_str(dssd)) == 5, "error");

int main(void)
{
    // Основной цикл

    defer(begin(), end())
    {
        PORTA_BITS.P0 = 0;
        PORTA_BITS.ALL = 5;
    }

    timer1_init(0xFA);

    struct reg rega = {
        .ddr = DDRA,
        .pin = PINA,
        .port = PORTA};

    while (1)
    {
        timer1_inc();
        PORTA_BITS.P0 = 0;
        PORTA_BITS.P0 = 1;
        PORTA = rega.port;
    }
}

__attribute__((weak)) void timer1_comp(void)
{
    PORTD = ~PORTD;
}

/* End File */