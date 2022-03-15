#include "timer.h"

static uint8_t timer_cnt = 0;
uint8_t timer_ocr = 0xFF;

/**
 * Функция инициализации программного таймера
 * Режим: CTC; TOP = timer_ocr
 * Источник тактового сигнала - переполнение таймера Т0
 * Время переполнения:
 * t = timer_ocr * 2.048 мс; f = 1 / timer_ocr / 2.048 Гц
 * 
 * \param ocr значение в блоке сравнения программного таймера
 */
void timer_init(uint8_t ocr)
{
    timer_cnt = 0;
    timer_ocr = ocr;
}

/**
 * Функция инкремента программного таймера в режиме CTC
 * При переполнении вызывается функция timer_comp()
 */
void timer_inc(void)
{
    timer_cnt++;
    if (timer_cnt >= timer_ocr)
    {
        timer_cnt = 0;
        timer_comp();
    }
}

__attribute__((weak)) void timer_comp(void)
{
}

/* End File */