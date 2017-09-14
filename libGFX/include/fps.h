#ifndef H_FPS
#define H_FPS

#include "libGFX.h"

#define PREFERRED_FPS 40

static Uint16 Tick_Interval = ((1000 / PREFERRED_FPS) / 10) * 10;
static Uint16 Min_Tick = ((1000 / PREFERRED_FPS) / 10) * 10;
static Uint32 Ticks = 0, Drawn_Frames = 0, next, now;

void print_fps();
void new_time();
int fps_ok();

#endif
