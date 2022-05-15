#include <avr/io.h>         
#include <avr/interrupt.h>  

#include "main.h"

ISR(ADC_vect)
{
    /**
     * Сброс флага OCF1B таймера Т1 (так как прерывания от таймера Т1 не
     * используются, то флаг необходимо сбрасывать программно)
     */
    TIFR |= (1 << OCF1B);

    adc_data = ADCH;
    flags.adc_data_rdy = 1;
}

/* End File */