#ifndef BALL_SYSTEM_H_INCLUDED
#define BALL_SYSTEM_H_INCLUDED

#include <SDL2/SDL.h>
#include "event_queue.h"

struct System
{
    struct Ball balls[100];
    int n;
};

void initSystem(struct System* s, int n);

void drawSystem(struct System *s, SDL_Renderer* renderer);

void step(struct System *s, double dt);

void predictSystem(double t, struct System* s, struct EventQueue* q);

void predictBall(double t, struct System* s, struct EventQueue* q, struct Ball* b);

#endif // BALL_SYSTEM_H_INCLUDED
