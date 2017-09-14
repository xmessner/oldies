#include "../include/primitives.h"

/*------------------------------------------------*/
/* procedures graphiques ecrivant dans la memoire */
/* et non directement a l'ecran			  */
/*------------------------------------------------*/
//OK
//void Mem_Pixel(int x,int y,unsigned char red,unsigned char green,unsigned char blue,BITMAP *VScreen)
void Mem_Pixel(int x,int y,unsigned char red,unsigned char green,unsigned char blue,SDL_Surface *VScreen)
    {
    Uint8	*ptr;
    
    ptr=VScreen->pixels;
    if(x<VScreen->w && x>0 && y<VScreen->h && y>0)
	{
	//A optimiser
//	*(ptr+=(y*VScreen->Pitch)+x)=Color;
	
	ptr+=y*VScreen->pitch+(x<<VScreen->format->BytesPerPixel);
	*ptr=blue;
	*(ptr+1)=green;
	*(ptr+2)=red;

	}
    }

//OK
//void Mem_Box(int xl,int yl,int xr,int yr,unsigned char red,unsigned char green,unsigned char blue,BITMAP *VScreen)
void Mem_Box(int xl,int yl,int xr,int yr,unsigned char red,unsigned char green,unsigned char blue,SDL_Surface *VScreen)
    {
    Uint8	*ptr,*ptr2;
    int		lenx,leny;
    int		swap;
    int		i;

    ptr=VScreen->pixels;
    
    //Si les coordonnees sont inversees
    if(xr<xl)
	{
	swap=xl;
	xl=xr;
	xr=swap;
	}
    if(yr<yl)
	{
	swap=yl;
	yl=yr;
	yr=swap;
	}
	
    //Calcul les distances
    //Depassement a droite
    if(xr>(VScreen->w+1))
	{
	lenx=VScreen->w-xl;
	}
    else
	{
	lenx=xr-xl;
	}
    //Depassement en bas
    if(yr>(VScreen->h+1))
	{
	leny=VScreen->h-yl;
	}
    else
	{
	leny=yr-yl;
	}
	
    if(xl<VScreen->w && yl<VScreen->h)
	{
	//On se positionne en haut a gauche
	//ptr+=yl*VScreen->pitch+(xl<<VScreen->format->BytesPerPixel);
	ptr+=(yl*VScreen->pitch)+(xl<<2);
	ptr2=ptr;
	//On dessine la ligne du haut
	for(i=0;i<lenx;i++)
	    {
	    //*ptr=Color;
	    *ptr=red;
	    *(ptr+1)=green;
	    *(ptr+2)=blue;
	    ptr+=VScreen->format->BytesPerPixel;
	    //ptr++;
	    }
	//On dessine la ligne de droite
	if(xr<VScreen->w)			//On est dans l'ecran
	    {
	    ptr-=VScreen->format->BytesPerPixel;
	    //ptr-=1;
	    for(i=0;i<leny;i++)
		{
		ptr+=VScreen->pitch;
		//*ptr=Color;
		*ptr=red;
		*(ptr+1)=green;
		*(ptr+2)=blue;
		}
	    }
	//On dessine la ligne de gauche
        for(i=0;i<leny;i++)
	    {
	    ptr2+=VScreen->pitch;
	    //*ptr2=Color;
	    *ptr2=red;
	    *(ptr2+1)=green;
	    *(ptr2+2)=blue;
	    }
	//On dessine la ligne du bas
	for(i=0;i<lenx;i++)
	    {
	    //*ptr2=Color;
	    *ptr2=red;
	    *(ptr2+1)=green;
	    *(ptr2+2)=blue;
	    ptr2+=VScreen->format->BytesPerPixel;
	    //ptr2++;
	    }
	}
    }
//OK	
//void Mem_Pbox(int xl,int yl,int xr,int yr,unsigned char red,unsigned char green, unsigned char blue,BITMAP *VScreen)
void Mem_Pbox(int xl,int yl,int xr,int yr,unsigned char red,unsigned char green,unsigned char blue,SDL_Surface *VScreen)
    {
    Uint8	*ptr,*ptr2;
    int		lenx,leny;
    int		swap;
    int		i,j;

    ptr=VScreen->pixels;
    //Si les coordonnees sont inversees
    if(xr<xl)
	{
	swap=xl;
	xl=xr;
	xr=swap;
	}
    if(yr<yl)
	{
	swap=yl;
	yl=yr;
	yr=swap;
	}
	
    //Calcul les distances
    //Depassement a droite
    if(xr>(VScreen->w+1))
	{
	lenx=VScreen->w-xl;
	}
    else
	{
	lenx=xr-xl;
	}
    //Depassement en bas
    if(yr>(VScreen->h+1))
	{
	leny=VScreen->h-yl;
	}
    else
	{
	leny=yr-yl;
	}
    //ptr+=yl*VScreen->Pitch+(xl<<VScreen->Bpp);
    ptr+=(yl*VScreen->pitch)+xl;
    //On dessine la boite
    ptr2=ptr;
    for(j=0;j<leny;j++)
	{
	ptr2+=VScreen->pitch;
	for(i=0;i<lenx;i++)
	    {
	    //*ptr=Color;
	    *ptr=blue;
	    *(ptr+1)=green;
	    *(ptr+2)=red;
	    ptr+=VScreen->format->BytesPerPixel;
	    //ptr++;
	    }
	ptr=ptr2;
	}
    }
    
/***********************************************************************
*  Line: Trace un segment dans la fenˆtre graphique en appliquant      *
*        l'algorithme de Bresenham                                     *
**--------------------------------------------------------------------**
*  Entr‚es : X1, Y1 = Coordonn‚es de l'origine                         *
*            X2, Y2 = Coordonn‚es de l'extr‚mit‚ terminale             *
*            COULEUR = couleur dusegment                               *
***********************************************************************/

/*-- Fonction accessoire pour ‚changer deux variables entiŠres -------*/
/*
void SwapInt( int *i1, int *i2 )
	{
	int dummy;
	dummy = *i2;  *i2   = *i1;  *i1   = dummy;
	}
*/
/*-- Proc‚dure principale -------------------------------------------*/
//OK
//void Mem_Line( int x1, int y1, int x2, int y2,unsigned char red,unsigned char green,unsigned char blue,BITMAP *VScreen)
//void Mem_Line( int x1, int y1, int x2, int y2,unsigned char Color,BITMAP *VScreen)
//	{
//	int d, dx, dy,
//	aincr, bincr,
//	xincr, yincr,
//	x, y;
//
//	if( abs(x2-x1) < abs(y2-y1) )     /* Sens du parcours : axe X ou Y ? */
//		{                                                           /* Par Y */
//		if ( y1 > y2 )                            /* y1 plus grand que y2? */
//			{
//			SwapInt( &x1, &x2 );                    /* Oui ‚change X1 et X2, */
//			SwapInt( &y1, &y2 );                                 /* Y1 et Y2 */
//			}
//			xincr = ( x2 > x1 ) ?  1 : -1;           /* Fixe le pas horizontal */
//
//			dy    = y2 - y1;
//			dx    = abs( x2-x1 );
//			d     = 2 * dx - dy;
//			aincr = 2 * (dx - dy);
//			bincr = 2 * dx;
//			x     = x1;
//			y     = y1;
//
//			//Mem_Pixel( x, y, red,green,blue, VScreen );               /* dessine le premier pixel */
//			Mem_Pixel( x, y, Color, VScreen );               /* dessine le premier pixel */
//			for( y=y1+1; y<= y2; ++y )                 /* Parcourt l'axe des Y */
//				{
//				if( d >= 0 )
//					{
//					x += xincr;
//					d += aincr;
//					}
//				else
//					d += bincr;
//				//Mem_Pixel( x, y, red,green,blue, VScreen );
//				Mem_Pixel( x, y, Color, VScreen );
//				}
//		}
//	else                                                        /* par X */
//		{
//		if( x1 > x2 )                             /* x1 plus grand que x2? */
//			{
//			SwapInt( &x1, &x2 );                    /* Oui, ‚change X1 et X2 */
//			SwapInt( &y1, &y2 );                                 /* Y1 et Y2 */
//			}
//
//		yincr = ( y2 > y1 ) ? 1 : -1;              /* Fixe le pas vertical */
///
//		dx    = x2 - x1;
//		dy    = abs( y2-y1 );
//		d     = 2 * dy - dx;
//		aincr = 2 * (dy - dx);
//		bincr = 2 * dy;
//		x     = x1;
//		y     = y1;
//
//		//Mem_Pixel( x, y, red,green,blue, VScreen );               /* Dessine le premier pixel */
//		for( x=x1+1; x<=x2; ++x )                  /* Parcourt l'axe des X */
//			{
//			if( d >= 0 )
//				{
//				y += yincr;
//				d += aincr;
//				}
//			else
//				d += bincr;
//			//Mem_Pixel( x, y, red,green,blue, VScreen );
//			Mem_Pixel( x, y, Color, VScreen );
//			}
//		}
//	}
//OK
/*int Rc_Copy(BITMAP *VScr_Source,int x1,int y1,int tx,int ty,BITMAP *VScr_Dest,int x2,int y2)
    {
    Uint8	*sptr,*sptr2,*dptr,*dptr2;
    int		i;
    
    if((x1>=VScr_Source->SizeX) || (x1< 0)) return;
    if((x2>=VScr_Dest->SizeX) || (x2 < 0)) return;
    if((y1>=VScr_Source->SizeY) || (y1 < 0)) return;
    if((y2>=VScr_Dest->SizeY) || (y2 < 0)) return;
    
    //Test les distances
    if((x2+tx)>=VScr_Dest->SizeX)
	{
	tx=VScr_Dest->SizeX-x2;
	}
    if((y2+ty)>=VScr_Dest->SizeY)
	{
	ty=VScr_Dest->SizeY-y2;
	}
    
    sptr=VScr_Source->AddOfBitmap;
    //sptr+=(y1*VScr_Source->Pitch)+(x1<<VScr_Source->Bpp);
    sptr+=(y1*VScr_Source->Pitch)+x1;
    sptr2=sptr;
    
    dptr=VScr_Dest->AddOfBitmap;
    //dptr+=(y2*VScr_Dest->Pitch)+(x2<<VScr_Dest->Bpp);
    dptr+=(y2*VScr_Dest->Pitch)+x2;
    dptr2=dptr;
    //tx<<=2;
    for(i=0;i<ty;i++)
	{
	memcpy(dptr,sptr,tx);
	sptr+=VScr_Source->Pitch;
	dptr+=VScr_Dest->Pitch;
	}
    return;
    }
*/	
/*void Rc_Copy(SDL_Surface *VScr_Source,int x1,int y1,int tx,int ty,SDL_Surface *VScr_Dest,int x3,int y3)
	{
	int i;
	for(i=0;i<ty;i++)
		{
		memcpy(VScr_Dest->pixels+(320*(y3+i))+x3,VScr_Source->pixels+(320*(y1+i))+x1,tx);
		}
	}
*/
// A FINIR
/*void Rc_Copy_Trans(BITMAP *VScr_Source,int x1,int y1,int tx,int ty,BITMAP *VScr_Dest,int x3,int y3,char trans)
	{
	int i=0,j=0;
	unsigned char Val;
	Uint8	*src,*dest;
	
	src=VScr_Source->AddOfBitmap;
	dest=VScr_Dest->AddOfBitmap;
	
	while(i!=ty)
		{
		while(j!=tx)
			{
			Val=*(src+((y1+i)<<8)+((y1+i)<<6)+(x1+j));
			if((x3+j)<320)
				{
				if(Val!=trans)
					{
					*(dest+((y3+i)<<8)+((y3+i)<<6)+(x3+j))=Val;
					j++;
					}
				else{ j++; }
				}
			else j++;
			}
		if((y3+i)<200) {i++;j=0;}
		else i=ty;
		}
	}

void Copy_Screen(BITMAP *VScr_Source,BITMAP *VScr_Dest)
	{
	//memcpy(VScr_Dest->AddOfBitmap,VScr_Source->AddOfBitmap,VScr_Source->SizeX*VScr_Source->SizeY<<VScr_Source->Bpp);
	memcpy(VScr_Dest->AddOfBitmap,VScr_Source->AddOfBitmap,VScr_Source->SizeX*VScr_Source->SizeY);
	}
*///OK
/*void Mem_ClrScr(BITMAP *VScreen)
	{
	//SDL_LockSurface(VScreen);
	//memset(VScreen->AddOfBitmap,0,VScreen->SizeX*VScreen->SizeY<<VScreen->Bpp);
	memset(VScreen->AddOfBitmap,0,VScreen->SizeX*VScreen->SizeY);
	//SDL_UnlockSurface(VScreen);
	}
*/
//OK
/********************************************
 * Affiche le buffer en parametre a l'ecran *
 ********************************************/
 
/*void Visu_Screen(BITMAP *VScreen)
    {
#ifdef DEV_SDL
//    Uint8	*sptr,*dptr;
//    int		i,ty;
//    
//    sptr=VScreen->AddOfBitmap;
//    dptr=screen->pixels;
//
//    ty=screen->h;
//    if(VScreen->SizeY<screen->h) ty=VScreen->SizeY;
//    for(i=0;i<ty;i++)
//	{
//	memcpy(dptr,sptr,VScreen->Pitch);
//	sptr+=VScreen->Pitch;
//	dptr+=screen->pitch;
//	}
    free(screen->pixels);
    screen->pixels=(unsigned char *)VScreen->AddOfBitmap;
    SDL_LockSurface(screen);
    SDL_UpdateRect(screen,0,0,screen->w,screen->h);
    SDL_UnlockSurface(screen);
#endif

#ifdef DEV_FB
	BITMAP *temp;
	// Ne pas depasser la taille max de l'ecran,
	// Sinon n'affiche pas
	if(VScreen->SizeX <= GScreen->SizeX)
	    {
	    if(VScreen->SizeY <= GScreen->SizeY)
		{
		temp=Malloc_Pic(640,480);
		Rc_Copy(VScreen,0,0,VScreen->SizeX,VScreen->SizeY,temp,(temp->SizeX>>1)-(VScreen->SizeX>>1),(temp->SizeY>>1)-(VScreen->SizeY>>1));
		write(FB,temp->AddOfBitmap,640*480);
		Free_Pic(temp);
		}
	    }
	lseek(FB,0L,0);
#endif

#ifdef DEV_SVGA
	unsigned int MidX,MidY;
	unsigned int tx=VScreen->SizeX,ty=VScreen->SizeY;	
	unsigned char	*sptr,*dptr;
	int		i;

	MidX=(DimX>>1)-(VScreen->SizeX>>1);
	MidY=(DimY>>1)-(VScreen->SizeY>>1);	
    
	//Test les distances
	if(MidX<0)
	    {
	    tx=DimX;MidX=0;
	    }
	if(MidY<0)
	    {
	    ty=DimY;MidY=0;
	    }
	sptr=VScreen->AddOfBitmap;
	dptr=graph_mem;
	dptr+=(MidY*DimX)+MidX;
	
	for(i=0;i<ty;i++)
	    {
	    memcpy(dptr,sptr,tx);
	    sptr+=VScreen->SizeX;
	    dptr+=DimX;
	    }


#endif
	}
//void Visu_Screen_Direct(BITMAP *VScreen)
//	{
//	BITMAP *temp;
//	temp=Malloc_Pic(640,480);
//	Rc_Copy(VScreen,0,0,356,480,temp,0,0);
//	write(FB,temp->AddOfBitmap,640*480);
//	lseek(FB,0L,0);
//	Free_Pic(temp);
//	}

void MV_Pal(unsigned char *Pal,BITMAP *VScreen)
    {
    int i,j,col=0;
    for(i=0;i<160;i+=10)
	{
	for(j=0;j<160;j+=10)
	    {
	    Mem_Pbox(i,j,i+10,j+10,col++,VScreen);
	    }
	}
    }*/