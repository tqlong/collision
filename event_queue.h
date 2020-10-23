#ifndef EVENT_QUEUE_H_INCLUDED
#define EVENT_QUEUE_H_INCLUDED

#include "ball.h"

#define QUEUE_CAPACITY 10000000

enum EventType
{
    DRAW_EVENT,
    VERTICAL_COLLISION_EVENT,
    HORIZONTAL_COLLISION_EVENT,
    BALL_COLLISION_EVENT,
    NONE_EVENT,
};

struct Event
{
    enum EventType type;
    double t;
    struct Ball *a; // pointer to first ball
    struct Ball *b; // pointer to second ball (null if wall collision)
    int count; // total collision count at the time of event creation
    // if this count is different from a->count + b->count
    // the the event is invalid
};

int isInvalid(struct Event e);

struct EventQueue
{
    struct Event *events;
    int n;
};

void initQueue(struct EventQueue* q);
void addToMinPQ(struct EventQueue* q, struct Event e);
struct Event deleteMin(struct EventQueue* q);

void addDrawEvent(struct EventQueue* q, double t);
void addVerticalCollisionEvent(struct EventQueue* q, double t, struct Ball* ball);
void addHorizontalCollisionEvent(struct EventQueue* q, double t, struct Ball* ball);

#endif // EVENT_QUEUE_H_INCLUDED
