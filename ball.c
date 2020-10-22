#include "ball.h"
#include "draw.h"

void drawBall(struct Ball* b, SDL_Renderer* renderer)
{
    int xc = (int) (b->x * SCREEN_SIZE);
    int yc = (int) (b->y * SCREEN_SIZE);
    int r  = (int) (b->weight * PIXEL_RADIUS_TO_WEIGHT_FACTOR);
    circleBres(xc, yc, r, b->color, renderer);
}

