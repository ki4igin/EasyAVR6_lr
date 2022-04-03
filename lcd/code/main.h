#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>
#include <avr/interrupt.h>

#include "lcd16x2.h"

struct user_flags
{
    uint8_t btn_lock : 1;
    uint8_t lcd_on : 1;
};

extern volatile struct user_flags flags __attribute__((io(&EEDR)));
// extern struct user_flags flags;

#endif

/* End File */