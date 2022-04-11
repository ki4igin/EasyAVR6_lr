#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <assert.h>
#include <avr/pgmspace.h>

#include "lcd16x2.h"

#include "timer.h"

#define buf_declare(_id, _name_index, _name_data, _size) \
    struct buf_##_id                                     \
    {                                                    \
        uint8_t _name_index;                             \
        uint8_t _name_data[_size];                       \
    } _id

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
#define get_str1(message) get_str(message)
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

// static_assert(1 == 2, get_str1(__TIME__) "asdfd");

#define MAX(x, y) ({                                   \
    __typeof__(x) my_localx = (x);                     \
    __typeof__(y) my_localy = (y);                     \
    my_localx > my_localy ? (my_localx) : (my_localy); \
})

#define qwerty(...) (1, ##__VA_ARGS__)  // ## нужно для работы макроса без аругументов, экранируется запятая


/**
 * PB7(SCK), PB5(MOSI), PB0(SS) - выходы (SS устанавливается в лог. 1)
 * PB6(MISO) - вход
 * Скорость обмена: F_CPU / 4; Режим: Master; CPOL = 0; CPHA = 0
 */
void spi_init(void)
{
    DDRB |= (1 << PB7) | (1 << PB5) | (1 << PB0);
    DDRB &= ~(1 << PB6);
    PORTB |= (1 << PB0);

    SPCR = (1 << SPE) | (1 << MSTR);
}

uint8_t spi_txrx(uint8_t tx_data)
{
    SPDR = tx_data;

    while ((SPSR & (1 << SPIF)) == 0)
        ;

    return SPDR;
}

int main(void)
{
    // Основной цикл

    uint8_t max_value = MAX(5, 4);
    PORTA = max_value;

    const char *str = PSTR("Hello World!");
    PORTA = (uint16_t)str;
    static_assert(sizeof(str) == 2, "error");

    defer(begin(), end())
    {
        PORTA_BITS.P0 = 0;
        PORTA_BITS.ALL = 5;
    }

    timer1_init(0xFA);

    lcd_disp_strpgm("STRasdasd sddddddddddd");

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