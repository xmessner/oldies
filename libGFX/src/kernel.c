#include "../include/kernel.h"

//Init du moteur SDL
int GFXInit(unsigned char GFXMode,unsigned char *WindowName)
	{
	const	SDL_VideoInfo *info;
	Uint32	flags;

	if( SDL_Init ( SDL_INIT_VIDEO ) < 0 ) {
		fprintf(stderr,"ERREUR INIT SDL : %s\n",SDL_GetError());
		return 1;}

	//En cas de sortie brutale    
        atexit(SDL_Quit);

	//Affiche un zoli texte pour la fenetre si sous X
	SDL_WM_SetCaption(WindowName, "");
	
	//Recupere les flags SDL
	flags = SDL_SWSURFACE|SDL_ANYFORMAT|SDL_DOUBLEBUF;
	
	/* Hardware acceleration is only used in fullscreen mode */
	//flags |= SDL_FULLSCREEN;

	/* Check for various video capabilities */
	info = SDL_GetVideoInfo();
	if ( info->blit_hw_CC && info->blit_fill ) {
		/* We use accelerated colorkeying and color filling */
		flags |= SDL_HWSURFACE;
	}
	else flags |= SDL_SWSURFACE;
	
	/* If we have enough video memory, and will use accelerated
	   blits directly to it, then use page flipping.
	 */
//	if ( (flags & SDL_HWSURFACE) == SDL_HWSURFACE ) {
//		/* Direct hardware blitting without double-buffering
//		   causes really bad flickering.
//		 */
//		if ( info->video_mem*1024 > (height*width*bpp/8) ) {
//			flags |= SDL_DOUBLEBUF;
//		} else {
//			flags &= ~SDL_HWSURFACE;
//		}
//	}
	
	//Choisi le mode grafik
    switch(GFXMode)
    	{
    	case GFX_640x480x8:
			SDLscreen=SDL_SetVideoMode(640,480,8,flags);
			break;
    	case GFX_800x600x8:
			SDLscreen=SDL_SetVideoMode(800,600,8,flags);
			break;
    	case GFX_1024x768x8:
			SDLscreen=SDL_SetVideoMode(1024,768,8,flags);
			break;
    	case GFX_640x480x16:
			SDLscreen=SDL_SetVideoMode(640,480,16,flags);
			break;
    	case GFX_800x600x16:
			SDLscreen=SDL_SetVideoMode(800,600,16,flags);
			break;
    	case GFX_1024x768x16:
			SDLscreen=SDL_SetVideoMode(1024,768,16,flags);
			break;
    	case GFX_640x480x24:
			SDLscreen=SDL_SetVideoMode(640,480,24,flags);
			break;
    	case GFX_800x600x24:
			SDLscreen=SDL_SetVideoMode(800,600,24,flags);
			break;
    	case GFX_1024x768x24:
			SDLscreen=SDL_SetVideoMode(1024,768,24,flags);
			break;
    	case GFX_640x480x32:
			SDLscreen=SDL_SetVideoMode(640,480,32,flags);
			break;
    	case GFX_800x600x32:
			SDLscreen=SDL_SetVideoMode(800,600,32,flags);
			break;
    	case GFX_1024x768x32:
			SDLscreen=SDL_SetVideoMode(1024,768,32,flags);
			break;
	//Mode 640*480*8
	default:
			SDLscreen=SDL_SetVideoMode(640,480,8,flags);
			break;
		}    
    if(SDLscreen==NULL) {
	fprintf(stderr,"ERREUR SetVideoMode %d : %s\n",GFXMode,SDL_GetError());
	return 1;}
	return 0;
	}
	
int GFXClose(void)
	{
	SDL_Quit();
	return 0;
	}
	
int GFXRefreshScreen(void)
	{
	SDL_Flip(SDLscreen);
//	SDL_UpdateRect(SDLscreen,0,0,0,0);
	}
	
int GFXRcCopy(SDL_Surface *src,int x1,int y1,int tx,int ty,SDL_Surface *dest,int x2,int y2)
    {
    SDL_Rect	RecSrc,RecDest;
	
    RecSrc.x=x1;
    RecSrc.y=y1;
    RecSrc.w=tx;
    RecSrc.h=ty;
	
    RecDest.x=x2;
    RecDest.y=y2;
    RecDest.w=tx;
    RecDest.h=ty;
    
    return(SDL_BlitSurface(src, &RecSrc, dest, &RecDest));
    }
    

