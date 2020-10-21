#include "draw.h"

void drawCircle(int xc, int yc, int x, int y, SDL_Color color, SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawPoint(renderer, xc+x, yc+y);
    SDL_RenderDrawPoint(renderer, xc-x, yc+y);
    SDL_RenderDrawPoint(renderer, xc+x, yc-y);
    SDL_RenderDrawPoint(renderer, xc-x, yc-y);
    SDL_RenderDrawPoint(renderer, xc+y, yc+x);
    SDL_RenderDrawPoint(renderer, xc-y, yc+x);
    SDL_RenderDrawPoint(renderer, xc+y, yc-x);
    SDL_RenderDrawPoint(renderer, xc-y, yc-x);
}

// Function for circle-generation
// using Bresenham's algorithm
void circleBres(int xc, int yc, int r, SDL_Color color, SDL_Renderer* renderer)
{
    int x = 0, y = r;
    int d = 3 - 2 * r;
    drawCircle(xc, yc, x, y, color, renderer);
    while (y >= x)
    {
        // for each pixel we will
        // draw all eight pixels

        x++;

        // check for decision parameter
        // and correspondingly
        // update d, x, y
        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
            d = d + 4 * x + 6;
        drawCircle(xc, yc, x, y, color, renderer);
    }
}

void clearScreen(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);
}
