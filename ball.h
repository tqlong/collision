#ifndef BALL_H_INCLUDED
#define BALL_H_INCLUDED

#include <SDL2/SDL.h>

#define SCREEN_SIZE 640
#define PIXEL_RADIUS_TO_WEIGHT_FACTOR 20.0
#define RADIUS_TO_WEIGHT_FACTOR (PIXEL_RADIUS_TO_WEIGHT_FACTOR / SCREEN_SIZE)

struct Ball {
    double x, y;   // 1, 1 <--> W, H, 0, 0 <--> 0, 0
    double vx, vy;
    double weight; // radius = weight * 20
    SDL_Color color;
    int count; // collision count to invalidate events
};

void drawBall(struct Ball* b, SDL_Renderer* renderer);

#endif // BALL_H_INCLUDED
