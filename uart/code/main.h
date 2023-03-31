#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>
#include "buf.h"

#define NBUF_TX 3
#define NBUF_RX 4

extern struct user_flags
{
    uint8_t btn_lock : 1;
    uint8_t rx_complete : 1;
    uint8_t tx_req : 1;
} flags;

extern buf_declare(buf_tx, NBUF_TX);
extern buf_declare(buf_rx, NBUF_RX);


extern uint8_t disp_num_byte;
extern uint8_t rx_data[NBUF_RX];

#endif

/* End File */