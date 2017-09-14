#ifndef H_KERNEL
#define H_KERNEL

#include "libGFX.h"

int GFXInit(unsigned char GFXMode,unsigned char *WindowName);
int GFXClose(void);
int GFXRefreshScreen(void);
int GFXRcCopy(SDL_Surface *src,int x1,int y1,int tx,int ty,SDL_Surface *dest,int x2,int y2);

#endif

