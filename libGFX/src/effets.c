#include "../include/effets.h"

void GFXInitFonte(FONTES *TabFontes)
	{
        int i;
        if((TabFontes->Coord=(COORD*)calloc(256,sizeof(COORD)))!=NULL)
                {
                for(i=0;i<256;i++)
		    {
		    TabFontes->Coord[i].X1=0;
		    TabFontes->Coord[i].Y1=0;
		    TabFontes->Coord[i].X2=0;
		    TabFontes->Coord[i].Y2=0;
		    }
		for(i=0;i<strlen(TabFontes->Alph);i++)
                        {
                        if((TabFontes->StartX+TabFontes->TailleX)>TabFontes->ImgFonte->w) 
			    {
			    TabFontes->StartX=0;
			    TabFontes->StartY=TabFontes->StartY+TabFontes->TailleY;
			    }
			    
                        //X1
                        TabFontes->Coord[(TabFontes->Alph[i])].X1=TabFontes->StartX;
                        //Y1
                        TabFontes->Coord[(TabFontes->Alph[i])].Y1=TabFontes->StartY;
                        //X2
                        TabFontes->Coord[(TabFontes->Alph[i])].X2=TabFontes->StartX+TabFontes->TailleX;
                        //Y2
                        TabFontes->Coord[(TabFontes->Alph[i])].Y2=TabFontes->StartY+TabFontes->TailleY;

                        TabFontes->StartX=TabFontes->StartX+TabFontes->TailleX;
                        }
                }

        }

void Put_Char(FONTES *TabFontes,unsigned char car,int x,int y,SDL_Surface *Dest,char mode)
	{
        GFXRcCopy(TabFontes->ImgFonte,
		TabFontes->Coord[car].X1,
		TabFontes->Coord[car].Y1,
		TabFontes->TailleX,
		TabFontes->TailleY,
		Dest,
		x,y);
	}

void Put_String(FONTES *TabFontes,unsigned char *ptr,int x,int y,SDL_Surface *Dest,char mode)
	{
	unsigned char car;
	while(*ptr!=0)
		{
		car=*ptr;
		Put_Char(TabFontes,car,x,y,Dest,mode);
		x=x+TabFontes->TailleX;
		*ptr++;
		}

	}

SCROLL *Init_Scroll(SDL_Surface *ImgFontes,SDL_Surface *ImgDest,FONTES *Fnt,char Vitesse,char Direction,unsigned char *Txt,unsigned int StartX,unsigned int StartY,unsigned int TailleX,unsigned int TailleY)
	{
	SCROLL *Scroll;
	Scroll=(SCROLL*)malloc(sizeof(SCROLL));

	Scroll->Txt=Txt;							// Pointeur sur la chaine de caracteres composant le texte
	Scroll->Index=0;							// Compteur de caracteres
	Scroll->Direction=Direction;						// Direction du scroll texte *** NUed ***
	Scroll->Vitesse=Vitesse;						// Vitesse de deplacement du scroll texte
	Scroll->Fnt=Fnt;							// Pointeur sur la structure de fontes ( X, Y, Taille .... )
	Scroll->ImgFontes=ImgFontes;						// Pointeur sur l'image des fontes
	Scroll->ImgDest=ImgDest;						// Pointeur sur l'image de destination //////
	Scroll->ImgScroll=(SDL_Surface *)SDL_CreateRGBSurface(SDL_SWSURFACE,TailleX,TailleY,32,0,0,0,0);		// Pointeur sur l'image source
	Scroll->StartX=StartX;							// X de depart du scroll *** NUed ***
	Scroll->StartY=StartY;							// Y de depart du scroll *** NUed ***
	Scroll->TailleX=TailleX;						// Taille en largeur du scroll
	Scroll->TailleY=TailleY;						// Taille en hauteur du scroll
	Scroll->Compteur=0;							// Compteur pour le bouclage des fontes
	return Scroll;
	}

FONTES* Malloc_Fontes(int TailleX,int TailleY,int StartX,int StartY,char *Alph,SDL_Surface *Bmp)
	{
	FONTES *Fnt;

	Fnt=(FONTES*)malloc(sizeof(FONTES));

	Fnt->TailleX=TailleX;
	Fnt->TailleY=TailleY;
	Fnt->StartX=StartX;
	Fnt->StartY=StartY;
	Fnt->Alph=Alph;
	Fnt->ImgFonte=Bmp;
	return Fnt;
	}

void Free_Fontes(FONTES *Fnt)
	{
	free(Fnt->Coord);
	SDL_FreeSurface(Fnt->ImgFonte);
	free(Fnt);
	}

void Scrolling(SCROLL *Scroll)
	{
	unsigned char	car;					// Variable contenant le caractere courant
	unsigned int	carx,cary;				// Debut de la fonte dans l'image en X et Y
	unsigned int	tcarx,tcary;				// Taille de la fonte dans l'image
    
	car=*(Scroll->Txt+Scroll->Index);				// On pointe sur le caractere suivant

	if(car==0)							// On est arrive a la fin du texte
		{
		Scroll->Index=0;
		car=*(Scroll->Txt+Scroll->Index);			// On pointe sur le debut du texte, on warp ;-)
		}

	carx=Scroll->Fnt->Coord[car].X1+Scroll->Compteur;
	cary=Scroll->Fnt->Coord[car].Y1;
	tcarx=Scroll->Fnt->TailleX;
	tcary=Scroll->Fnt->TailleY;
    
	// Copie du gros bloc a sa destination
	GFXRcCopy(Scroll->ImgScroll,
	Scroll->Vitesse,
	0,
	Scroll->TailleX,
	Scroll->TailleY,
	Scroll->ImgScroll,
	0,0);
	    
	// Copie du morceau de la lettre
	GFXRcCopy(Scroll->ImgFontes,
	carx,cary,
	tcarx,tcary,
	Scroll->ImgScroll,
	Scroll->TailleX-Scroll->Vitesse,
	0);
	    
	// On recopie le scroll dans la destination
	GFXRcCopy(Scroll->ImgScroll,0,0,
	Scroll->TailleX,Scroll->TailleY,
	Scroll->ImgDest,
	Scroll->StartX,Scroll->StartY);
	    
	// Incrementation du compteur
	// Ce compteur doit etre augmente d'un multiple de la largeur de la fonte
	// Scroll->Vitesse est un multiple de la largeur de la fonte
	Scroll->Compteur+=Scroll->Vitesse;
	if((Scroll->Fnt->TailleX-Scroll->Compteur) == 0) { Scroll->Index++; Scroll->Compteur=0;}	// On repasse le compteur a sa valeur initiale
	}	