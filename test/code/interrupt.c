#include <avr/io.h>
#include <avr/interrupt.h>

#include "timer.h"

ISR(INT0_vect)
{
    // User code
}

ISR(INT1_vect)
{
    timer_inc();  // User code
}

void timer_comp(void)
{
    PORTB = ~PORTB;
}

/* End File */