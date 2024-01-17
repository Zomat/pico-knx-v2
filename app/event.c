#include "event.h"

Event event_queue[MAX_EVENTS];

static int front = 0, rear = 0;

int event_enqueue(Event e)
{
    if ((rear + 1) % MAX_EVENTS == front) {
        return -1;
    }

    event_queue[rear] = e;
    rear = (rear + 1) % MAX_EVENTS;
    return 0;
}

Event event_dequeue()
{
    Event e;

    if (front == rear) {
        e.type = -1;
        return e;
    }

    e = event_queue[front];
    front = (front + 1) % MAX_EVENTS;

    if (front == rear) {
        front = rear = 0;
    }

    return e;
}

void print_events()
{
    printf("Event Queue Contents:\n");

    int current_index = front;
    while (current_index != rear) {
        Event e = event_queue[current_index];
        printf("Type: %d, Data: %p\n", e.type, e.data);
        current_index = (current_index + 1) % MAX_EVENTS;
    }
}