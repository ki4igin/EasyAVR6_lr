#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>

#define NBUF_TX 3
#define NBUF_RX 4

extern struct user_flags
{
    uint8_t btn_lock : 1;
    uint8_t rx_complete : 1;
    uint8_t tx_req : 1;
} flags;

extern struct buf_tx
{
    uint8_t count;
    uint8_t data[NBUF_TX];
} tx;

extern struct buf_rx
{
    uint8_t count;
    uint8_t data[NBUF_RX];
} rx;

extern struct buf_pac
{
    uint8_t num_byte;
    uint8_t data[NBUF_RX];
} pac;

#endif

/* End File */