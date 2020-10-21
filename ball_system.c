#include "ball_system.h"
#include "event_queue.h"

#include <stdlib.h>

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

void predictBall(double t, struct System* s, struct EventQueue* q, struct Ball* ball)
{
    double dt;
    double radius = ball->weight * 20 / SCREEN_SIZE;

    if (ball->vx < 0)
    {
        dt = (radius - ball->x) / ball->vx;
    }
    else
    {
        dt = (1 - radius - ball->x) / ball->vx;
    }
    addVerticalCollisionEvent(q, t+dt, ball);

    if (ball->vy < 0)
    {
        dt = (radius - ball->y) / ball->vy;
    }
    else
    {
        dt = (1 - radius - ball->y) / ball->vy;
    }
    addHorizontalCollisionEvent(q, t+dt, ball);
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
