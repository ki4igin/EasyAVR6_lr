#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>

#define TIMERS \
    X(1)       \
    X(2)       \
    X(3)       \
    X(4)       \
    X(5)       \
    X(6)       \
    X(7)

#define X(num)                           \
                                         \
    extern uint8_t timer##num##_ocr;     \
                                         \
    void timer##num##_init(uint8_t ocr); \
    void timer##num##_inc(void);         \
                                         \
    void timer##num##_comp(void);

TIMERS

#undef X

#endif

/* End File */