#ifndef SEVSEG_H
#define SEVSEG_H

#include <avr/io.h>

void sevseg_init(void);
void sevseg_bin2bcd(uint16_t data, uint8_t *digs);
void sevseg_disp(const uint8_t *digs);
void sevseg_off(void);

#endif

/* End File */