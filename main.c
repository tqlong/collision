#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "draw.h"
#include "ball.h"
#include "event_queue.h"
#include "ball_system.h"

#define DRAW_INTERVAL 0.03

void processDrawEvent(struct System* s, struct EventQueue* q, SDL_Renderer* renderer, struct Event e)
{
    addDrawEvent(q, e.t+DRAW_INTERVAL);

    drawSystem(s, renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay( (int)(DRAW_INTERVAL*1000) );

}

void processVerticalCollisionEvent(struct System* s, struct EventQueue* q, struct Event e)
{
    e.a->vx = -e.a->vx;
    e.a->count++;
    predictBall(e.t, s, q, e.a);
}

void processHorizontalCollisionEvent(struct System* s, struct EventQueue* q, struct Event e)
{
    e.a->vy = -e.a->vy;
    e.a->count++;
    predictBall(e.t, s, q, e.a);
}

void mainLoop(SDL_Window* window, SDL_Renderer* renderer)
{
	int n = 10;
    double t = 0;
	struct System s;
	struct EventQueue q;

	initSystem(&s, n);
	initQueue(&q);
    addDrawEvent(&q, 0);
	predictSystem(0, &s, &q);

    SDL_Event event;
    while( 1 ) {
        if( SDL_PollEvent( &event ) && event.type == SDL_QUIT ) break;

        clearScreen(renderer);

        struct Event e = deleteMin(&q);
        printf("event type=%d t=%.5f c=%d q.n=%d\n", e.type, e.t, e.count, q.n);

        if (isInvalid(e)) continue;
        step(&s, e.t - t);

        switch (e.type) {
        case DRAW_EVENT:
            processDrawEvent(&s, &q, renderer, e);
            break;

        case VERTICAL_COLLISION_EVENT:
            processVerticalCollisionEvent(&s, &q, e);
            break;

        case HORIZONTAL_COLLISION_EVENT:
            processHorizontalCollisionEvent(&s, &q, e);
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

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	} else {
		window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            SCREEN_SIZE, SCREEN_SIZE, 0);
		if( window == NULL ) {
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		} else {
			renderer = SDL_CreateRenderer(window, -1, 0);
            mainLoop(window, renderer);
		}
	}

	SDL_DestroyWindow( window );
	SDL_Quit();

	return 0;
}

