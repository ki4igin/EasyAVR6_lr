#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>

extern volatile struct user_flags
{
    uint8_t adc_data_rdy : 1;
    uint8_t sig_conv_busy : 1;    
} flags __attribute__((io(&EEDR)));

extern uint16_t adc_data;

#endif

/* End File */