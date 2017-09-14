#include "../include/libGFX.h"

//Fonctions du programme
int main(int argc,char **argv)
	{
	int i;
	long Taille_Fic;
	FONTES *TabFontes;
	SCROLL *Scroll;
	SDL_Surface	*image,*ImgFontes;
	
	FILE *fic=fopen("../data/txt.dat","r");
	char fonte[]="../data/fonte.tga";
	char alph[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,'!:-?();+* ";
	unsigned char text[1000];
	char result,warp=0;
	char Vitesse;                 //Valeurs 1,2,4,8
	unsigned char *ptr,*Ptr_Txt;
	
	
	GFXInit(10);
	image=GFXLoadPKM("../data/tmp.pkm");

	memset(text,0,1000);
	Taille_Fic=File_Size(fic);
	for(i=0;i<(Taille_Fic-2);i++)
		{
		fread(&text[i],1,1,fic);
		}
	fclose(fic);

	ImgFontes=(SDL_Surface*)GFXLoadTGA(fonte);
	TabFontes=Malloc_Fontes(16,16,0,0,alph,ImgFontes);

	GFXInitFonte(TabFontes);
	Scroll=Init_Scroll(ImgFontes,SDLscreen,TabFontes,2,0,text,0,0,320,17);

	
	GFXRcCopy(image,0,0,320,200,SDLscreen,(640/2)-(320/2),(480/2)-(200/2));
	for(i=0;i<500;i++){
	new_time();
	Scrolling(Scroll);
	if(fps_ok())
		GFXRefreshScreen();}
	GFXClose();
	SDL_FreeSurface(image);
	SDL_FreeSurface(ImgFontes);
	return 0;
	}
