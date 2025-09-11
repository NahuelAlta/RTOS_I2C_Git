#ifndef MAX30100_H
#define MAX30100_H
#include "stdint.h"
#include "stdbool.h"

uint8_t     max30100_write_register         (uint8_t reg, uint8_t value) ;
uint8_t     max30100_read_register          (uint8_t reg);
uint8_t     max30100_read_interrupt_status  ();
bool        max30100_enable_interrupts      (uint8_t interrupt_mask);
void        max30100_init_with_interrupts   ();

#endif