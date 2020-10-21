#ifndef __DRAW_H
#define __DRAW_H

#include <SDL2/SDL.h>

void clearScreen(SDL_Renderer* renderer);

void drawCircle(int xc, int yc, int x, int y, SDL_Color color, SDL_Renderer* renderer);

void circleBres(int xc, int yc, int r, SDL_Color color, SDL_Renderer* renderer);

#endif // __DRAW_H

