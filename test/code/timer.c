#include "timer.h"



#define X(num)                                         \
                                                       \
    static uint8_t timer##num##_cnt = 0;               \
    uint8_t timer##num##_ocr = 0xFF;                   \
                                                       \
    void timer##num##_init(uint8_t ocr)                \
    {                                                  \
        timer##num##_cnt = 0;                          \
        timer##num##_ocr = ocr;                        \
    }                                                  \
                                                       \
    void timer##num##_inc(void)                        \
    {                                                  \
        timer##num##_cnt++;                            \
        if (timer##num##_cnt >= timer##num##_ocr)      \
        {                                              \
            timer##num##_cnt = 0;                      \
            timer##num##_comp();                       \
        }                                              \
    }                                                  \
                                                       \
    __attribute__((weak)) void timer##num##_comp(void) \
    {                                                  \
    }
TIMERS

#undef X

/* End File */