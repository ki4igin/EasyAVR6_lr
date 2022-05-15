#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>

extern volatile struct user_flags
{
    uint8_t adc_data_rdy : 1;
} flags __attribute__((io(&EEDR)));

extern uint8_t adc_data;

#endif

/* End File */