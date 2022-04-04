#include <avr/io.h>         
#include <avr/interrupt.h>  

#include "main.h"
#include "lcd16x2.h"

/* Прерывание используется для обработки нажатий кнопок */
ISR(TIMER2_OVF_vect)
{
    if ((PINA & (1 << PINA5)) == 0)
    {
        if (flags.btn_lock == 0)
        {
            flags.btn_lock = 1;

            if (flags.lcd_on)
            {
                flags.lcd_on = 0;
                lcd_send_cmd(1 << LCD_ON);
            }
            else
            {
                flags.lcd_on = 1;
                lcd_send_cmd((1 << LCD_ON) | (1 << LCD_ON_D));
            }
        }
    }
    else
    {
        flags.btn_lock = 0;
    }
}

/* End File */