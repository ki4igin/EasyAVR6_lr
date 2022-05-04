#ifndef TWI_H
#define TWI_H

#include <avr/io.h>

enum twi_status
{
    TWI_NOT_INIT = 0,
    TWI_READY,
    TWI_TX_BUSY,
    TWI_TX_COMPLETE,
    TWI_RX_BUSY,
    TWI_RX_COMPLETE,
    TWI_ERROR,
};

extern volatile enum twi_status twi_status;

void twi_init(void);
void twi_write(uint8_t slaw, uint8_t reg_addr, uint8_t *buf, uint8_t size);
void twi_read(uint8_t slaw, uint8_t reg_addr, uint8_t *buf, uint8_t size);

#endif

/* End File */