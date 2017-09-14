#include "../include/fileio.h"

//Fonctions lecture fichier graphique
long File_Size(FILE *stream)
	{
	long curpos, length;

	curpos = ftell(stream);
	fseek(stream, 0L, SEEK_END);
	length = ftell(stream);
	fseek(stream, curpos, SEEK_SET);
	return length;
	}

SDL_Surface* GFXLoadPKM(char *Img_Name)
	{
	FILE		*tmp;
	struct		Img_PKM iPKM;
	SDL_Surface	*TempSurface;
	unsigned char	*BitmapPKM;
	unsigned char	*Pal;

	unsigned char	*Pack_Buff;

	long		Taille_Fichier;
	char		PKM[]="PKM";
        
	unsigned char	swap;
	unsigned char	swap2;

	int		i;
	//Uint32		IndPal,IndImg,IndRGB;
	unsigned char	*pixel;
	Uint8		*RGB;
	
	//Ouverture du fichier
	FILE		*fic=fopen(Img_Name,"rb");
	//On recupere la taille du fichier
	Taille_Fichier=File_Size(fic);

	//Lecture du HEADER
	//lecture de la signature du fichier PKM
	fread(iPKM.Signature,3,1,fic);
	iPKM.Signature[3]=0;
	if(strcmp(iPKM.Signature,PKM)==0)
		{
		//recuperation du header
		fread(&iPKM.Version,1,1,fic);        //version
		fread(&iPKM.Pack_byte,1,1,fic);      //pack_byte
		fread(&iPKM.Pack_word,1,1,fic);      //pack_word

		//recuperation de la taille de l'image
		//Largeur
		fread(&swap,1,1,fic);
		fread(&swap2,1,1,fic);
		iPKM.Largeur=swap2;
		iPKM.Largeur=iPKM.Largeur << 8;
		iPKM.Largeur=iPKM.Largeur+swap;

		//Hauteur
		fread(&swap,1,1,fic);
		fread(&swap2,1,1,fic);
		iPKM.Hauteur=swap2;
		iPKM.Hauteur=iPKM.Hauteur << 8;
		iPKM.Hauteur=iPKM.Hauteur+swap;

		//Allocation du Bitmap
		BitmapPKM=(unsigned char*)malloc(iPKM.Largeur*iPKM.Hauteur);
		//Reservation de la surface SDL => A Modifier en fonction de la profondeur de l'ecran
		//TempSurface=SDL_CreateRGBSurface(SDL_SWSURFACE,iPKM.Largeur,iPKM.Hauteur,32,0,0,0,0);
		TempSurface=SDL_CreateRGBSurface(SDL_SWSURFACE,iPKM.Largeur,iPKM.Hauteur,24,0,0,0,0);
		//Allocation de la palette
		Pal=(unsigned char *)malloc(768);
		
		//recuperation de la palette de couleur
		fread(Pal,sizeof(unsigned char),768,fic);

		//Convertion palette 63->255 SDL/Linux
		for(i=0;i<768;i++)
			{
			Pal[i]<<=2;
			}

		//lecture du Post-Header
		fseek(fic,778,0);
                
		fread(&swap,1,1,fic);
		fread(&swap2,1,1,fic);
                
		iPKM.Taille_PH=swap2;
		iPKM.Taille_PH=iPKM.Taille_PH << 8;
		iPKM.Taille_PH=iPKM.Taille_PH+swap;

		//on saute le Post-Header
		for(i=0;i<iPKM.Taille_PH;i++) fread(&swap2,1,1,fic);

		iPKM.Taille_image=iPKM.Largeur*iPKM.Hauteur;
		iPKM.Taille_donnees=Taille_Fichier-(780+iPKM.Taille_PH);

		//Reservation de l'espace memoire pour le bloc
		Pack_Buff=(char *)malloc(iPKM.Taille_donnees);
		
		//Lecture du Bloc Compresse
		fread(Pack_Buff,sizeof(char),iPKM.Taille_donnees,fic);

		//On appel la routine de decompression
		Unpack_PKM(&iPKM,BitmapPKM,Pack_Buff);
		//Fermeture du fichier
		fclose(fic);
    		
		//Convertion 32 bits
		pixel=BitmapPKM;
		RGB=TempSurface->pixels;
		for(i=0;i<(iPKM.Largeur*iPKM.Hauteur);i++)
			{
			//R
			*(RGB + 2)=Pal[ (*pixel * 3)];
			//G
			*(RGB + 1)=Pal[ (*pixel * 3)+1];
			//B
			*RGB=Pal[ (*pixel * 3)+2];    
			//A
			//RGB+=4;
			RGB+=3;
			pixel++;
			}
		}
	return TempSurface;
	}

void Unpack_PKM(struct Img_PKM *iPKM,unsigned char *VScreen,unsigned char *Pack_Buff)
	{
	long Compteur_donnees;
	long Compteur_pixels;
	unsigned char Couleur;
	unsigned char Octet_lu;
	int Offset=0;
	int Word_lu;
	int Compteur;
	long Taille_Fichier;

	unsigned char swap;
	unsigned char swap2;

	Compteur_donnees=0;
	Compteur_pixels=0;

	while((Compteur_pixels < iPKM->Taille_image) && (Compteur_donnees < iPKM->Taille_donnees))
		{
		Octet_lu=Pack_Buff[Offset++];

		if((Octet_lu!=iPKM->Pack_byte) && (Octet_lu!=iPKM->Pack_word))
			{
			VScreen[Compteur_pixels]=Octet_lu;
			Compteur_pixels++;
			Compteur_donnees++;
			}
		else
			{
			if(Octet_lu == iPKM->Pack_byte)
				{
				Couleur=Pack_Buff[Offset++];
				Octet_lu=Pack_Buff[Offset++];
				
				for(Compteur=0;Compteur<(Octet_lu);Compteur++)
					{
					VScreen[Compteur_pixels+Compteur]=Couleur;
					}
				Compteur_pixels=Compteur_pixels+Octet_lu;
				Compteur_donnees=Compteur_donnees+3;
				}
			else 
				{
				Couleur=Pack_Buff[Offset++];
				swap=Pack_Buff[Offset++];
				swap2=Pack_Buff[Offset++];

				Word_lu=swap;
				Word_lu=Word_lu<<8;
				Word_lu=Word_lu+swap2;

				for(Compteur=0;Compteur<(Word_lu);Compteur++)
				VScreen[Compteur_pixels+Compteur]=Couleur;
				Compteur_pixels=Compteur_pixels+Word_lu;
				Compteur_donnees=Compteur_donnees+4;
				}
			}
		}
	}
#define endi(p) ((p)[0] + ((p)[1]<<8))
SDL_Surface *GFXLoadTGA(char *Img_Name)
	{
	SDL_Surface	*Img;
	struct Img_TGA	iTGA;
	unsigned char	*RGB;
	
	FILE *Fic=fopen(Img_Name,"rb");
	
	fread(&iTGA,sizeof(struct Img_TGA),1,Fic);

	fprintf(stderr,"TGA Info *********\n");
	fprintf(stderr,"infolen : %d\n",iTGA.Infolen);
	fprintf(stderr,"HasCmap : %d\n",iTGA.HasCmap);
	fprintf(stderr,"type : %d\n",iTGA.Type);	
	fprintf(stderr,"cmapbits : %d\n",iTGA.CmapBits);
	fprintf(stderr,"infolen : %d\n",iTGA.Flags);

	fprintf(stderr,"BPP : %d\n",iTGA.Bpp);
	switch(iTGA.Bpp)
	    {
	    case 8:
		Img=_GFXLoadTGA256(&iTGA,Fic);
		break;
	    case 24:
		Img=_GFXLoadTGA24bits(&iTGA,Fic);
		break;
	    case 32:
		Img=_GFXLoadTGAtrueColor(&iTGA,Fic);
		break;
	    default:
		fprintf(stderr,"LoadTGA : Format pas prit en compte\n");
		break;
	    }
	    
	fclose(Fic);
	return Img;
	}

SDL_Surface *_GFXLoadTGA256(struct Img_TGA *iTGA,FILE *Fic)
	{
	SDL_Surface	*Img;
	unsigned int	tx,ty,cmaplen,i,j;
	unsigned char	*Pal;
	unsigned char 	*temp;
	unsigned char	r,g,b,swap;
	unsigned char	*ptr,*pixel;
		
	tx=endi(iTGA->Width);
	ty=endi(iTGA->Height);
	cmaplen=endi(iTGA->CmapLen);
	Pal=(unsigned char*)malloc(768);

	Img=(SDL_Surface *)SDL_CreateRGBSurface(SDL_SWSURFACE,tx,ty,8,0,0,0,0);
	temp=(unsigned char *)malloc(tx*ty);
	ptr=temp;
	
	// Chargement de la palette
	fseek(Fic,18,0);
	fread(Pal,1,cmaplen*3,Fic);
		
	// Convertion palette BGR->RVB
	j=0;
	for(i=0;i<cmaplen;i++)
	    {
	    r=Pal[j++];
	    j++;
	    swap=Pal[j++];
	    Pal[j-1]=r;
	    Pal[j-3]=swap;
	    }
	
	// Chargement des pixels	    
	for(i=0;i<(tx*ty);i++)
	    {
	    fread(ptr,1,1,Fic);
	    ptr++;
	    }
	//Conversion 32bits
//	pixel=temp;
//	RGB=Img->pixels;
//	for(i=0;i<(tx*ty);i++)
//		{
//		//R
//		*(RGB + 2)=Pal[ (*pixel * 3)];
//		//G
//		*(RGB + 1)=Pal[ (*pixel * 3)+1];
//		//B
//		*RGB=Pal[ (*pixel * 3)+2];    
//		//A
//		RGB+=4;
//		pixel++;
//		}
	free(Pal);
	free(temp);
	return Img;
	}

SDL_Surface* _GFXLoadTGAtrueColor(struct Img_TGA *iTGA,FILE *Fic)
	{
	SDL_Surface	*Img;
	unsigned int	tx,ty,cmaplen,i,j;
	unsigned char	*Pal;
	unsigned char	*ptr;
		
	tx=endi(iTGA->Width);
	ty=endi(iTGA->Height);
	cmaplen=endi(iTGA->CmapLen);
	Pal=(unsigned char*)malloc(768);

	Img=(SDL_Surface *)SDL_CreateRGBSurface(SDL_SWSURFACE,tx,ty,32,0,0,0,0);
	ptr=Img->pixels;
	
	// Chargement de la palette
	fseek(Fic,18,0);
	fread(Pal,1,cmaplen*3,Fic);
		
	// Chargement des pixels BGR
	for(i=0;i<(tx*ty*4);i++)
	    {
	    fread(ptr,1,1,Fic);
	    ptr++;
	    }
	free(Pal);
	return Img;
	}

SDL_Surface* _GFXLoadTGA24bits(struct Img_TGA *iTGA,FILE *Fic)
	{
	SDL_Surface	*Img;
	unsigned int	tx,ty,cmaplen,i,j;
	unsigned char	*Pal;
	unsigned char	*ptr;
		
	tx=endi(iTGA->Width);
	ty=endi(iTGA->Height);
	cmaplen=endi(iTGA->CmapLen);
	Pal=(unsigned char*)malloc(768);

	Img=(SDL_Surface *)SDL_CreateRGBSurface(SDL_SWSURFACE,tx,ty,24,0,0,0,0);
	ptr=Img->pixels;
	
	// Chargement de la palette
	fseek(Fic,18,0);
//	fread(Pal,1,cmaplen*3,Fic);
		
	// Chargement des pixels BGR
	for(i=0;i<(tx*ty*3);i++)
	    {
	    fread(ptr,1,1,Fic);
	    ptr++;
	    }
	free(Pal);
	return Img;
	}

unsigned int GFXSaveTGA(SDL_Surface *Image,unsigned char *Name)
    {
    FILE *Fic;
    unsigned char *src;
    struct Img_TGA iTGA;
//    unsigned char tmp[3];
    unsigned short t;
    unsigned char r,g,b;
    unsigned char Pal[768];
    
    Fic=fopen(Name,"wb");
    
    iTGA.Infolen=0;
    iTGA.HasCmap=0;
    iTGA.Type=2;			//32 bits
    iTGA.CmapStart[0]=0;
    iTGA.CmapStart[1]=0;

    iTGA.CmapLen[0]=256 % 256;
    iTGA.CmapLen[1]=256 / 256;
//    fprintf(stderr,"*** %d\n",endi(iTGA.CmapLen));
    
    
    iTGA.CmapBits=0;
    iTGA.Xorigin[0]=0;
    iTGA.Xorigin[1]=0;
    iTGA.Yorigin[0]=0;
    iTGA.Yorigin[1]=0;

    iTGA.Width[0]=Image->w % 256;
    iTGA.Width[1]=(Image->w / 256);
//    fprintf(stderr,"*** %d\t*** %d\n",endi(iTGA.Width),Image->SizeX);

    iTGA.Height[0]=Image->h  % 256;
    iTGA.Height[1]=(Image->h / 256);
//    fprintf(stderr,"*** %d\t*** %d\n",endi(iTGA.Height),Image->SizeY);

    iTGA.Bpp=24;
    iTGA.Flags=32;
    fwrite(&iTGA,sizeof(struct Img_TGA),1,Fic);
//    for(t=0;t<768;t++)
//	{
//	r=Image->Pal[t*3];
//	g=Image->Pal[t*3+1];
//	b=Image->Pal[t*3+2];
//	fwrite(&b,1,1,Fic);
//	fwrite(&g,1,1,Fic);
//	fwrite(&r,1,1,Fic);
//	}
    memset(Pal,0,768);
    fwrite(Pal,768,1,Fic);
    
    fwrite(Image->pixels,Image->w*Image->h*4,1,Fic);
    fclose(Fic);
    return 0;
    }

SDL_Surface *GFXMallocPic(int SizeX,int SizeY)
    {
    SDL_Surface	*Temp;
    
    Temp=SDL_CreateRGBSurface(SDL_SWSURFACE,SizeX,SizeY,32,0,0,0,0);

    return Temp;
    }