#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>
#include <avr/interrupt.h>

#include "sevseg.h"
#include "timer.h"

struct user_flags
{
    uint8_t is_btn_lock : 1;
    uint8_t led_dir : 1;
};

extern struct user_flags flags;

#endif

/* End File */