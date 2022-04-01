#include <avr/io.h>         
#include <avr/interrupt.h>  

#include "main.h"
#include "lcd16x2.h"

// Handlers --------------------------------------------------------------------
/*******************************************************************************
Timer2 Overflow Handler
Используется для обработки нажатия кнопок
*******************************************************************************/
ISR(TIMER2_OVF_vect)
{
    if (!(PINA & (1 << PINA5)))
    {
        if (flag.btn_lock == 0)
        {
            flag.btn_lock = 1;

            if (flag.lcd_on)
            {
                flag.lcd_on = 0;
                lcd_send_cmd(1 << LCD_ON);
            }
            else
            {
                flag.lcd_on = 1;
                lcd_send_cmd((1 << LCD_ON) | (1 << LCD_ON_D));
            }
        }
    }
    else
    {
        flag.btn_lock = 0;
    }
}

// End File --------------------------------------------------------------------