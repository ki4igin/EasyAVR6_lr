#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>

extern uint8_t timer_ocr;  // Значение в блоке сравнения програмного таймера

void timer_init(uint8_t ocr);
void timer_inc(void);

void timer_comp(void);

#endif

/* End File */