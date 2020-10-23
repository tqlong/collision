#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "draw.h"
#include "ball.h"
#include "event_queue.h"
#include "ball_system.h"
#include "duration.h"

#define DRAW_INTERVAL 0.03
clock_t last_clock;

void processDrawEvent(struct System* s, struct EventQueue* q, SDL_Renderer* renderer, struct Event e)
{
    clock_t now, duration, delay = DRAW_INTERVAL*1000;

    addDrawEvent(q, e.t+DRAW_INTERVAL);

    drawSystem(s, renderer);
    SDL_RenderPresent(renderer);

    now = clock();
    duration = now - last_clock;
    last_clock = now;

    delay = delay > duration ? delay - duration : 0;
    SDL_Delay( delay );
    // SDL_Delay( 1 );
}

void processVerticalCollisionEvent(struct System* s, struct EventQueue* q, struct Event e)
{
    e.a->vx = -e.a->vx;
    e.a->count++; // increase count to invalidate events created for this ball
    predictBall(e.t, s, q, e.a);
}

void processHorizontalCollisionEvent(struct System* s, struct EventQueue* q, struct Event e)
{
    e.a->vy = -e.a->vy;
    e.a->count++; // increase count to invalidate events created for this ball
    predictBall(e.t, s, q, e.a);
}

void processBallCollistionEvent(struct System* s, struct EventQueue* q, struct Event e)
{
    // compute new velocities of e.a and e.b
    struct Ball *a = e.a, *b = e.b;
    double x = b->x - a->x, y = b->y - a->y;
    double vx = b->vx - a->vx, vy = b->vy - a->vy;
    double ra = a->weight * RADIUS_TO_WEIGHT_FACTOR;
    double rb = b->weight * RADIUS_TO_WEIGHT_FACTOR;
    double dvdr = vx*x + vy*y;
    double dist = ra+rb;
    double magnitude = 2 * a->weight * b->weight * dvdr / ((a->weight + b->weight) * dist);
    double fx = magnitude * x / dist;
    double fy = magnitude * y / dist;

    double old_total_energy = (a->vx*a->vx + a->vy*a->vy)*a->weight + (b->vx*b->vx + b->vy*b->vy)*b->weight;
    double new_total_energy, factor;

    a->vx += fx / a->weight;
    a->vy += fy / a->weight;
    b->vx -= fx / b->weight;
    b->vy -= fy / b->weight;

    // to maintain energy conservation
    new_total_energy =  (a->vx*a->vx + a->vy*a->vy)*a->weight + (b->vx*b->vx + b->vy*b->vy)*b->weight;
    factor = sqrt(old_total_energy / new_total_energy);
    a->vx *= factor;
    a->vy *= factor;
    b->vx *= factor;
    b->vy *= factor;

    e.a->count++; // increase count to invalidate events created for
    e.b->count++; // the two balls
    predictBall(e.t, s, q, e.a);
    predictBall(e.t, s, q, e.b);
}

void mainLoop(SDL_Window* window, SDL_Renderer* renderer)
{
    int n = 1000;
    double t = 0;
    struct System s;
    struct EventQueue q;
    last_clock = clock();
    tick();

    initSystem(&s, n);
    initQueue(&q);
    addDrawEvent(&q, 0);
    predictSystem(0, &s, &q);
    printf("Init time = %.2f sec q.n = %d\n", tick_tock(), q.n);

    SDL_Event event;
    while( 1 )
    {
        if( SDL_PollEvent( &event ) && event.type == SDL_QUIT ) break;

        clearScreen(renderer);

        struct Event e = deleteMin(&q);
        // printf("event type=%d t=%.5f c=%d q.n=%d\n", e.type, e.t, e.count, q.n);

        if (isInvalid(e)) continue;
        if (e.type != NONE_EVENT) step(&s, e.t - t);

        switch (e.type)
        {
        case DRAW_EVENT:
            processDrawEvent(&s, &q, renderer, e);
            printf("step time = %.2f sec q.n = %d\n", tick_tock(), q.n);
            break;

        case VERTICAL_COLLISION_EVENT:
            processVerticalCollisionEvent(&s, &q, e);
            break;

        case HORIZONTAL_COLLISION_EVENT:
            processHorizontalCollisionEvent(&s, &q, e);
            break;

        case BALL_COLLISION_EVENT:
            processBallCollistionEvent(&s, &q, e);
            break;

        default:
            break;
        }
        t = e.t;
    }
}

int main( int argc, char* args[] )
{
    srand(time(0));
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }
    else
    {
        window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                   SCREEN_SIZE, SCREEN_SIZE, 0);
        if( window == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        }
        else
        {
            renderer = SDL_CreateRenderer(window, -1, 0);
            mainLoop(window, renderer);
        }
    }

    SDL_DestroyWindow( window );
    SDL_Quit();

    return 0;
}

