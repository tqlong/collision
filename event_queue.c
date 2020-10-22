#include "event_queue.h"
#include <stdlib.h>

int isInvalid(struct Event e)
{
    int count = (e.a ? e.a->count : 0) + (e.b ? e.b->count : 0);
    return count != e.count;
}

void addDrawEvent(struct EventQueue* q, double t)
{
    struct Event e;
    e.type = DRAW_EVENT;
    e.t = t;
    e.a = NULL;
    e.b = NULL;
    e.count = 0;
    addToMinPQ(q, e);
}

void addVerticalCollisionEvent(struct EventQueue* q, double t, struct Ball* ball)
{
    struct Event e;
    e.type = VERTICAL_COLLISION_EVENT;
    e.t = t;
    e.a = ball;
    e.b = NULL;
    e.count = ball->count;
    addToMinPQ(q, e);
}

void addHorizontalCollisionEvent(struct EventQueue* q, double t, struct Ball* ball)
{
    struct Event e;
    e.type = HORIZONTAL_COLLISION_EVENT;
    e.t = t;
    e.a = ball;
    e.b = NULL;
    e.count = ball->count;
    addToMinPQ(q, e);
}

void initQueue(struct EventQueue* q)
{
    q->events = malloc(QUEUE_CAPACITY*sizeof(struct Event));
    q->n = 0;
}

void exch(struct EventQueue* q, int i, int j)
{
    struct Event e;
    e = q->events[i];
    q->events[i] = q->events[j];
    q->events[j] = e;
}

void swim(struct EventQueue* q, int i)
{
    while (i > 0)
    {
        int r = (i-1) / 2;
        if (q->events[r].t <= q->events[i].t) break;
        exch(q, i, r);
        i = r;
    }
}

void sink(struct EventQueue* q, int i)
{
    while (i*2+1 < q->n)
    {
        int r = i*2+1;
        if (r+1 < q->n && q->events[r].t > q->events[r+1].t) ++r;
        if (q->events[i].t <= q->events[r].t) break;
        exch(q, i, r);
        i = r;
    }
}

void addToMinPQ(struct EventQueue* q, struct Event e)
{
    q->events[q->n++] = e;
    swim(q, q->n-1);
}

struct Event deleteMin(struct EventQueue* q)
{
    struct Event e = q->events[0];
    q->events[0] = q->events[--(q->n)];
    sink(q, 0);
    return e;
}
