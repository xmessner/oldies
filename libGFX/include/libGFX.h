#ifndef H_MAIN
#define H_MAIN

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>

//Variables Globales
SDL_Surface *SDLscreen;

//Variable d'evenement
SDL_Event event;

//Definitions globales

//Resolutions Graphiques
//8bits
#define	GFX_640x480x8	1
#define	GFX_800x600x8	2
#define	GFX_1024x768x8	3
//16bits
#define	GFX_640x480x16	4
#define	GFX_800x600x16	5
#define	GFX_1024x768x16	6
//24bits
#define	GFX_640x480x24	7
#define	GFX_800x600x24	8
#define	GFX_1024x768x24	9
//32bits
#define	GFX_640x480x32	10
#define	GFX_800x600x32	11
#define	GFX_1024x768x32	12
//OpenGL

#include "kernel.h"
#include "primitives.h"
#include "effets.h"
#include "fileio.h"
#include "fps.h"
//#include "event.h"
//#include "events.h"

#endif
