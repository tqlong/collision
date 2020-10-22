#include "ball_system.h"
#include "event_queue.h"

#include <stdlib.h>
#include <math.h>

void initSystem(struct System* s, int n)
{
    s->n = n;
    for (int i = 0; i < n; i++)
    {
        s->balls[i].x = (double) rand() / RAND_MAX * 0.8 + 0.1;
        s->balls[i].y = (double) rand() / RAND_MAX * 0.8 + 0.1;
        s->balls[i].weight = 1.0;
        s->balls[i].vx = (double) rand() / RAND_MAX * 0.1 + 0.1;
        s->balls[i].vy = (double) rand() / RAND_MAX * 0.1 + 0.1;
        if (rand() % 2 == 0) s->balls[i].vx = -s->balls[i].vx;
        if (rand() % 2 == 0) s->balls[i].vy = -s->balls[i].vy;
        s->balls[i].color.r = rand()%156+100;
        s->balls[i].color.g = rand()%156+100;
        s->balls[i].color.b = rand()%156+100;
        switch (rand() % 3) {
            case 0: s->balls[i].color.r = 0; break;
            case 1: s->balls[i].color.g = 0; break;
            default: s->balls[i].color.g = 0; break;
        }
        s->balls[i].color.a = 255;
        s->balls[i].count = 0;
    }
}

void drawSystem(struct System *s, SDL_Renderer* renderer)
{
//    printf("n = %d\n", s->n);
    for (int i = 0; i < s->n; i++)
    {
        drawBall(&s->balls[i], renderer);
//        printf("%f %f\n", s->balls[i].x, s->balls[i].y);
    }
}

void step(struct System *s, double dt)
{
    for (int i = 0; i < s->n; i++)
    {
        s->balls[i].x = s->balls[i].x + dt * s->balls[i].vx;
        s->balls[i].y = s->balls[i].y + dt * s->balls[i].vy;
    }
}

struct Event getVerticalCollisionEvent(double t, struct Ball* ball)
{
    struct Event e;
    double dt;
    double radius = ball->weight * RADIUS_TO_WEIGHT_FACTOR;

    // vertical wall events
    if (ball->vx < 0)
    {
        dt = (radius - ball->x) / ball->vx;
    }
    else
    {
        dt = (1 - radius - ball->x) / ball->vx;
    }

    e.type = VERTICAL_COLLISION_EVENT;
    e.t = t+dt;
    e.a = ball;
    e.b = NULL;
    e.count = ball->count;
    return e;
}

struct Event getHorizontalCollisionEvent(double t, struct Ball* ball)
{
    struct Event e;
    double dt;
    double radius = ball->weight * RADIUS_TO_WEIGHT_FACTOR;
    // horizontal wall events
    if (ball->vy < 0)
    {
        dt = (radius - ball->y) / ball->vy;
    }
    else
    {
        dt = (1 - radius - ball->y) / ball->vy;
    }

    e.type = HORIZONTAL_COLLISION_EVENT;
    e.t = t+dt;
    e.a = ball;
    e.b = NULL;
    e.count = ball->count;
    return e;
}

double getCollisionTime(struct Ball* a, struct Ball* b)
{
    double x = b->x - a->x, y = b->y - a->y;
    double vx = b->vx - a->vx, vy = b->vy - a->vy;
    double ra = a->weight * RADIUS_TO_WEIGHT_FACTOR;
    double rb = b->weight * RADIUS_TO_WEIGHT_FACTOR;

    double _b = vx*x + vy*y;
    double _a = vx*vx + vy*vy;
    double _c = x*x+y*y - (ra+rb)*(ra+rb);
    double delta = _b*_b - _a*_c;
    if (_b > 0 || _a == 0 || delta < 0) return -1;

    return -(_b + sqrt(delta)) / _a;
}

struct Event getCollisionEvent(double t, struct Ball* a, struct Ball* b)
{
    struct Event e;
    double dt;

    e.type = NONE_EVENT;
    e.t = t;
    e.a = a;
    e.b = b;
    e.count = a->count + b->count;

    if (a == b) return e;
    dt = getCollisionTime(a, b);
    if (dt >= 0)
    {
        e.type = BALL_COLLISION_EVENT;
        e.t += dt;
    }
    return e;
}

void predictBall(double t, struct System* s, struct EventQueue* q, struct Ball* ball)
{
    addToMinPQ(q, getVerticalCollisionEvent(t, ball));
    addToMinPQ(q, getHorizontalCollisionEvent(t, ball));

    // ball collision events
    for (int i = 0; i < s->n; i++)
    {
        struct Event e = getCollisionEvent(t, ball, &s->balls[i]);
        if (e.type == BALL_COLLISION_EVENT)
            addToMinPQ(q, e);
    }
}

void predictSystem(double t, struct System* s, struct EventQueue* q)
{
    // vertical wall collision event
    for (int i = 0; i < s->n; i++)
    {
        struct Ball *ball = &s->balls[i];
        predictBall(t, s, q, ball);
    }
}
