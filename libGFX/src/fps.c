#include "../include/fps.h"

void print_fps()
{
  printf("Preferred FPS was: %i\n", PREFERRED_FPS);
  printf("Real FPS was : %i\n", 1000 / (Ticks / Drawn_Frames));
}

void new_time()
{
     next = SDL_GetTicks() + Tick_Interval;
}

int fps_ok()
{
  now = SDL_GetTicks();
  Ticks += Tick_Interval;
  
  if (now < next)
    {
      /*frame rate is met*/
      SDL_Delay(next - now);
    }
  else if (next != now)
    {
      /*drop frame*/
      return 0;
    }

  Drawn_Frames++;
  return 1;
}


