#include "duration.h"

clock_t last_tick;

clock_t tick()
{
    last_tick = clock();
    return last_tick;
}

double tock(clock_t start)
{
    clock_t now = tick();
    return (double)(now-start) / CLOCKS_PER_SEC;
}

double tick_tock()
{
    return tock(last_tick);
}
