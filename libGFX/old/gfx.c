#include <stdio.h>
#include <stdlib.h>

#include "../../config.h"

#ifdef DEV_SDL
#include <SDL/SDL.h>
#endif

#ifdef DEV_FB
#include <fcntl.h>
#include <sys/io.h>
#include <linux/fb.h>
#endif

#ifdef DEV_SVGA
#include <vga.h>
#include <vgagl.h>
#endif

#include "../../include/MXcommon.h"
#include "../../include/MXgfx.h"

/*---------------------------------*/
/* procedures graphiques generales */
/*---------------------------------*/
#ifdef DEV_FB
unsigned short red[256],green[256],blue[256];
struct fb_cmap SAVcmap={0,256,red,green,blue};
#endif

void Set_Color(unsigned char color,unsigned char red,unsigned char green,unsigned char blue)
    {
//    outb(color,0x3c8);
//    outb(red,0x3c9);
//    outb(green,0x3c9);
//    outb(blue,0x3c9);
#ifdef DEV_SVGA
    vga_setpalette(color,red,green,blue);
#endif
    }

void Set_Pal(unsigned char *pal)
    {
#ifdef DEV_FB
    unsigned int i;
    unsigned int j=0;
    
    unsigned short red[256],green[256],blue[256];
    struct fb_cmap cmap={0,256,red,green,blue};
    for(i=0;i<256;i++)
	{
	red[i]=(*(pal++))<<8;
	green[i]=(*(pal++))<<8;
	blue[i]=(*(pal++))<<8;
	}	
    if(ioctl(FB,FBIOPUTCMAP,&cmap)==-1) printf("ERREUR PUTCMAP !!\n");
#endif

#ifdef DEV_SVGA
    int i,j=0;
    unsigned char r,g,b;    	

    for(i=0;i<256;i++)
	{
	r=pal[j]>>2;
	g=pal[j+1]>>2;
	b=pal[j+2]>>2;
	j+=3;
	vga_setpalette(i,r,g,b);
	}
#endif
    }

void Pal_Black(unsigned char *pal,char mode)
	{
	int i;
	for(i=0;i<768;i++) pal[i]=0;
	if(mode!=0) {Wait_Vbl();Set_Pal(pal);}
	}

#ifdef DEV_FB
void Get_Pal(void)
	{
	if(ioctl(FB,FBIOGETCMAP,&SAVcmap)==-1) printf("ERREUR GETCMAP !!\n");
	}
#endif

#ifdef DEV_SVGA
void Get_Pal(void)
    {
    }
#endif

void Fade_Out(unsigned char *pal)
	{
	int i,fin=0;
	while(fin!=1)
		{
		fin=1;
		for(i=0;i<768;i++)
			{
			if(pal[i]>0) { pal[i]=pal[i]-1;fin=0;}
			}
		Wait_Vbl();
		Set_Pal(pal);
		}
	}

void Fade_In(unsigned char *Pal)
        {
        unsigned char *TmpPalette;
	unsigned char *Palette;
	int i,j;
        TmpPalette=(unsigned char *)malloc(768);
	Palette=(unsigned char *)malloc(768);
        Pal_Black(Palette,0);

        /***** creation de la palette inverse *****/
	for(i=0;i<768;i++) TmpPalette[i]=Pal[i];

        /***** Fade in *****/
        for(i=0;i<255;i++)
		{
		for(j=0;j<768;j++)
			{
                        if(TmpPalette[j]<255) {TmpPalette[j]=TmpPalette[j]+1;}
                        else Palette[j]=Palette[j]+1;
			}
		Wait_Vbl();
		Set_Pal(Palette);
		}
	free(Palette);
	free(TmpPalette);
	}


void Fade_To(unsigned char *spal,unsigned char *dpal)
	{
	int i,fin=0;
	while(fin!=1)
		{
		fin=1;
		for(i=0;i<768;i++)
			{
			if(spal[i]<dpal[i]) {spal[i]=spal[i]+1;fin=0;}
			else {
				if(spal[i]>dpal[i]) {spal[i]=spal[i]-1;fin=0;}
                                if(spal[i]==dpal[i]);
				 }
			}
		Wait_Vbl();
		Set_Pal(spal);
		}
	}

/*--------------------------------------------------------------------*/
/* Fonction realisant le cyclage d'une palette donnee                 */
/* *pal      : pointeur sur la palette a animer                       */
/* col1      : couleur de depart                                      */
/* col2      : couleur de fin ( col1 < col2 )                         */
/* direction : 0 -> col1 vers col2                                    */
/* 	       1 -> col2 vers col1                                    */
/* mode      : 0 -> animation de la palette de 1 unite dans direction */
/* 	     : 1 -> animation de la palette tant que pas de touche    */
/* speed     : 1 -> 8 ( 1..8*100ms)                                   */
/*--------------------------------------------------------------------*/

void Cycle_Pal(unsigned char *pal,unsigned char col1,unsigned char col2,char direction,char mode)
	{
	unsigned char savecol[3];
	unsigned char swap[3];
	unsigned char a;                          	//a contient la couleur courante
	if(mode!=1)
		{
		if(direction!=1)
			{
			a=(col2-1)*3;
			savecol[0]=pal[a];
			savecol[1]=pal[a+1];
			savecol[2]=pal[a+2];
			while(col1<a)
				{
				a-=3;
				swap[0]=pal[a];
				swap[1]=pal[a+1];
				swap[2]=pal[a+2];
				pal[a+3]=swap[0];
				pal[a+4]=swap[1];
				pal[a+5]=swap[2];
				}
			pal[a]=savecol[0];
			pal[a+1]=savecol[1];
			pal[a+2]=savecol[2];
			}
		}
	}

void Vga_On(void)
    {
#ifdef DEV_SDL
    //Init du mode 640x480x8 pour le moment
    if( SDL_Init ( SDL_INIT_VIDEO ) < 0 )
	{
	fprintf(stderr,"ERREUR INIT SDL : %s\n",SDL_GetError());
	exit(1);
	}
    atexit(SDL_Quit);
    screen=SDL_SetVideoMode(640,480,8,SDL_SWSURFACE);
    if(screen==NULL)
	{
	fprintf(stderr,"ERREUR Setvideo : %s\n",SDL_GetError());
	exit(1);
	}
#endif

#ifdef DEV_FB
    //Attribution des droits pour inb
    iopl(3);
    //Ouverture du FrameBuffer
    if((FB=open("/dev/fb0",O_WRONLY))==-1)
	{
	printf("ERREUR D'OUVERTURE DU FRAMEBUFFER\n");
	exit(1);
	}
    //Recuperation de la taille ecran
    //Allocation du buffer ecran
    GScreen=(BITMAP *)Malloc_Pic(640,480);
    // Sauvegarde de la palette d'origine
    Get_Pal();
    // On efface le curseur
//    printf("\033[?25h\033[?0c");
#endif

#ifdef DEV_SVGA
    vga_init();
    vga_setmode(G640x480x256); // Pour le moment !!
    vga_ext_set(VGA_EXT_SET,VGA_CLUT8);
    vga_setpage(0);
    vga_setlinearaddressing();
    graph_mem=vga_getgraphmem();
    DimX=640;DimY=480;
    GScreen=(BITMAP*)Malloc_Pic(640,480);
#endif
    }

void Vga_Off(void)
        {
#ifdef DEV_SDL
    SDL_Quit();
#endif

#ifdef DEV_FB
	// Restauration de la palette d'origine
	if(ioctl(FB,FBIOPUTCMAP,&SAVcmap)==-1) printf("ERREUR PUTCMAP\n");

	free(GScreen);
	close(FB);
	// On affiche le curseur
//	printf("\033[?25l\033[?1c");
#endif

#ifdef DEV_SVGA
	Free_Pic(GScreen);
	vga_setmode(TEXT);
#endif
        }

void Wait_Vbl(void)
    {
#ifdef DEV_FB
    while(!(inb(0x3da)&8));
    while((inb(0x3da)&8));
#endif

#ifdef DEV_SVGA
     vga_waitretrace();
#endif
    }