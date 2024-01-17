#ifndef _EVENT_HANDLER_H_
#define _EVENT_HANDLER_H_

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include <stdio.h>
#include "app/event.h"
#include "app/knx.h"
#include "app/oled.h"

bool handle_knx_switch_event(void *data);
bool handle_knx_set_address_event(void *data);
bool handle_knx_dimming_event(void *data);

#endif //_EVENT_HANDLER_H_