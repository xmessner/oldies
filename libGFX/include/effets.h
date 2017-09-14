#ifndef H_EFFETS
#define H_EFFETS

#include "libGFX.h"

typedef struct Coord  {
				int		X1;
				int		Y1;
				int		X2;
				int		Y2;
				}COORD;

typedef struct Fontes {
				int			StartX,StartY;		//Depart de la fonte X et Y
				int			TailleX,TailleY;	//Taille de la fonte
				char			*Alph;			//Alphabet de la fonte
				int			*TabCoord;		//Coordonnees des caracteres
				SDL_Surface		*ImgFonte;		//image de la fonte
				COORD		*Coord;
				}FONTES;
				
typedef struct ScrollTxt {
				FONTES		*Fnt;			/* structure de la fonte */
				SDL_Surface	*ImgFontes;		/* image de la fonte (provisoire) */
				SDL_Surface		*ImgScroll;		/* Image contenant le scrolling */
				SDL_Surface		*ImgDest;		/* Image de destination */
				unsigned char	*Txt;			/* pointeur sur le texte */
				char		Warp;			/* fin de texte */
				char		Vitesse;		/* Vitesse du scrolling */
				char		Direction;		/* Direction du scrolling */
				unsigned int		TailleX;
				unsigned int		TailleY;
				unsigned int		StartX;
				unsigned int		StartY;
				unsigned int		Index;
				unsigned int		Compteur;
				}SCROLL;

void GFXInitFonte(FONTES *TabFontes);
void Put_Char(FONTES *TabFontes,unsigned char car,int x,int y,SDL_Surface *Dest,char mode);
void Put_String(FONTES *TabFontes,unsigned char *ptr,int x,int y,SDL_Surface *Dest,char mode);
SCROLL *Init_Scroll(SDL_Surface *ImgFontes,SDL_Surface *ImgDest,FONTES *Fnt,char Vitesse,char Direction,unsigned char *Txt,unsigned int StartX,unsigned int StartY,unsigned int TailleX,unsigned int TailleY);
void Scrolling(SCROLL *Scroll);
FONTES* Malloc_Fontes(int TailleX,int TailleY,int StartX,int StartY,char *Alph,SDL_Surface *Bmp);
void Free_Fontes(FONTES *Fnt);

#endif