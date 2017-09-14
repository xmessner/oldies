#include "../include/libGFX.h"

//Fonctions du programme
int main(int argc,char **argv)
	{
	int i;
	SDL_Surface	*image;
	
	GFXInit(10);
	image=GFXLoadPKM("tmp.pkm");
	GFXRcCopy(image,0,0,320,200,SDLscreen,110,410);
	GFXRefreshScreen();
	SDL_Delay(3000);
	GFXClose();
	SDL_FreeSurface(image);
	SDL_FreeSurface(SDLscreen);
	return 0;
	}