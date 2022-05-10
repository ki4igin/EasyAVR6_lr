#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>

struct user_flags
{
    uint8_t adc_conv_complete : 1;
};

extern volatile struct user_flags flags __attribute__((io(&EEDR)));;
extern uint8_t adc_data;

#endif

/* End File */