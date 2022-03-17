#include <avr/io.h>
#include <avr/interrupt.h>

#include "timer.h"

ISR(INT0_vect)
{
    // User code
}

ISR(INT1_vect)
{
    timer1_inc();  // User code
}

void timer1_comp(void)
{
    PORTB = ~PORTB;
}

/* End File */