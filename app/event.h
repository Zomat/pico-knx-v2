#ifndef _EVENT_H_
#define _EVENT_H_

#include <stdio.h>

#define MAX_EVENTS 10

typedef enum {
  EMPTY_EVENT,
  KNX_SWITCH_EVENT,
  KNX_SET_ADDR_EVENT,
  KNX_DIMMING_EVENT
} EventType;

typedef struct {
  EventType type;
  void *data;
} Event;

extern Event event_queue[MAX_EVENTS];

int event_enqueue(Event e);

Event event_dequeue();

void print_events();

#endif //_EVENT_H_