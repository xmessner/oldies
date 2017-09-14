BITMAP* Load_PKM(char *Img_Name)
	{
	FILE		*tmp;
	struct		Img_PKM iPKM;
	BITMAP		*BitmapPKM;

	char		*Pack_Buff;

	long		Taille_Fichier;
	char		PKM[]="PKM";
        
	unsigned char	swap;
	unsigned char	swap2;

	int		i;
	//Uint32		IndPal,IndImg,IndRGB;
	unsigned char	*pixel;
	//Uint8		*RGB;
	
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
		BitmapPKM=(BITMAP*)Malloc_Pic(iPKM.Largeur,iPKM.Hauteur);

		//recuperation de la palette de couleur
		fread(BitmapPKM->Pal,sizeof(unsigned char),768,fic);

		//Convertion palette 63->255 SDL/Linux
		for(i=0;i<768;i++)
			{
			BitmapPKM->Pal[i]<<=2;
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
		//pixel=BitmapPKM->AddOfBitmap;
		//RGB=BitmapPKM->SDLbitmap->pixels;
		//for(i=0;i<(iPKM.Largeur*iPKM.Hauteur);i++)
		//    {
		//    //R
		//    *(RGB + 2)=BitmapPKM->Pal[ (*pixel * 3)];
		//    //G
		//    *(RGB + 1)=BitmapPKM->Pal[ (*pixel * 3)+1];
		//    //B
		//    *RGB=BitmapPKM->Pal[ (*pixel * 3)+2];
		//    
		//    RGB+=4;
		//    pixel++;
		//    }
		}
	return BitmapPKM;
	}

void Unpack_PKM(struct Img_PKM *iPKM,BITMAP *VScreen,char *Pack_Buff)
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
			VScreen->AddOfBitmap[Compteur_pixels]=Octet_lu;
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
					VScreen->AddOfBitmap[Compteur_pixels+Compteur]=Couleur;
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
					VScreen->AddOfBitmap[Compteur_pixels+Compteur]=Couleur;
				Compteur_pixels=Compteur_pixels+Word_lu;
				Compteur_donnees=Compteur_donnees+4;
				}
			}
		}
	}