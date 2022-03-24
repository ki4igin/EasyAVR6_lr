#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>

#define NBUF_TX 3
#define NBUF_RX 4

typedef struct
{
    uint8_t is_btn_lock : 1;
    uint8_t rx : 1;
    uint8_t tx : 1;
} Flags_t;

extern uint8_t bufTx[NBUF_TX];
extern uint8_t bufRx[NBUF_RX];
extern uint8_t dataRx[NBUF_RX];

extern Flags_t flags;

// extern uint8_t cntTx;

typedef struct
{
    union
    {
        uint8_t size;
        uint8_t num;
    };
    uint8_t data[3];
} buf_t;

extern struct
{
    union
    {
        uint8_t size;
        uint8_t num;
    };
    uint8_t data[3];
} tx_buf;

// extern struct buffer tx_buf;

#endif

/* End File */