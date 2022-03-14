#include <avr/io.h>         
#include <avr/interrupt.h>  

#include "main.h"
#include "sevseg.h"  
#include "timer.h"   

/* Прерывание используется для обработки нажатий кнопок */
ISR(TIMER2_OVF_vect)
{    
    if ((PIND & (1 << PIND0)) == 0)
    {
        OCR1BL++;
    }
    else if (!(PIND & (1 << PIND1)) == 0)
    {
        OCR1BL--;
    }
    else if ((PINB & (1 << PINB5)) == 0)
    {
        if (flags.is_btn_lock == 0)
        {
            flags.is_btn_lock = 1;
            flags.led_dir ^= 1;
        }
    }
    else if ((PINB & (1 << PINB6)) == 0)
    {
        timer_ocr--;
    }
    else if ((PINB & (1 << PINB7)) == 0)
    {
        timer_ocr++;
    }
    else
    {
        flags.is_btn_lock = 0;
    }
}

/**
 * Прерывание используется для инкремента программного таймера и вывода
 * информации на семисегментные индикаторы
 */
ISR(TIMER0_OVF_vect)
{
    timer_inc();

    uint8_t kd = OCR1BL * 100 / 256;
    uint8_t digits[4];  

    sevseg_bin2bcd(kd, digits);
    sevseg_display_process(digits);
}

/**
 * Событие от программного таймера Используется для циклического сдвига
 * положения "бегущего огонька"
 */
void timer_comp(void)
{
    uint8_t temp = PORTA;
    if (flags.led_dir)
    {
        temp >>= 1;
        temp = temp ? temp : 0x80;
    }
    else
    {
        temp <<= 1;
        temp = temp ? temp : 0x01;
    }
    PORTA = temp;
}

/* End File */