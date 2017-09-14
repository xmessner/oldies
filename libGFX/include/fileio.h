#ifndef H_FILEIO
#define H_FILEIO

#include "libGFX.h"

struct Img_PKM	{
                
			long Taille_image;
			long Taille_donnees;
			char Signature[4];
			char Version;
			char Pack_byte;
			char Pack_word;
			int Largeur;
			int Hauteur;
			int Taille_PH;
                };

struct Img_TGA {
		unsigned char	Infolen;
		unsigned char	HasCmap;
		unsigned char	Type;
		
		unsigned char	CmapStart[2];
		unsigned char	CmapLen[2];
		unsigned char	CmapBits;
		
		unsigned char	Yorigin[2];
		unsigned char	Xorigin[2];
		unsigned char	Width[2];
		unsigned char	Height[2];
		unsigned char	Bpp;
		unsigned char	Flags;
//		unsigned char	*Pal;
		};

                
//Public
SDL_Surface* GFXLoadPKM(char *Img_Name);
SDL_Surface *GFXLoadTGA(char *Img_Name);
SDL_Surface *GFXMallocPic(int TailleX,int TailleY);
unsigned int GFXSaveTGA256(SDL_Surface *Image,unsigned char *Name);

//Private
void Unpack_PKM(struct Img_PKM *iPKM,unsigned char *VScreen,unsigned char *Pack_Buff);
long File_Size(FILE *stream);
SDL_Surface* _GFXLoadTGA256(struct Img_TGA *iTGA,FILE *Fic);
SDL_Surface* _GFXLoadTGAtrueColor(struct Img_TGA *iTGA,FILE *Fic);
SDL_Surface* _GFXLoadTGA24bits(struct Img_TGA *iTGA,FILE *Fic);

#endif