#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>

#define NBUF_TX 3
#define NBUF_RX 4

struct user_flags
{
    uint8_t btn_lock : 1;
    uint8_t rx_complete : 1;
    uint8_t tx_req : 1;
};

extern struct user_flags flags;

extern uint8_t tx_count;
extern uint8_t tx_data[NBUF_TX];
extern uint8_t rx_count;
extern uint8_t rx_data[NBUF_RX];
extern uint8_t qqq_count;
extern uint8_t qqq_data[NBUF_RX];

#endif

/* End File */