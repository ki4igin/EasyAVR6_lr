#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>         // Заголовочный файл для работы с РВВ МК
#include <avr/interrupt.h>  // Заголовочный файл для работы с прерываниями МК

#include "lcd16x2.h"  // Заголовочный файл для работы с дисплеем LCD16x2

// Typedef ---------------------------------------------------------------------
// Тип пользовательских флагов
typedef struct
{
    uint8_t btn_lock : 1;  // Флаг нажатия кнопки
    uint8_t lcd_on : 1;  // Флаг включения экрана дисплея
} Flags_t;

register Flags_t flag asm("r2");  // Переменная пользовательских флагов в РОН r2

#endif

/* End File */