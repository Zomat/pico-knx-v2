#ifndef _KNX_H_
#define _KNX_H_

#include "pico/stdlib.h"

/**
 * UART Settings
 */
#define UART_ID uart1
#define BAUD_RATE 19200
#define UART_TX_PIN 4
#define UART_RX_PIN 5

extern char knx_source_address[];
extern char knx_target_address[];

bool knx_switch(bool state);
bool knx_dimming(uint8_t value);

#endif _KNX_H_