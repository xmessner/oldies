#include "entete.h"
#include "digitdrums_data.h"

/************************************************
 *                                              *
 * Charge et initialise la structure de donnees *
 *                                              *
 ************************************************/
YMData *InitSong(unsigned char *Name,int NumChip)
    {
    YMData	*Song;
    int		Ret=1;
    int		i;

    Song=LoadSong(Name);

    Song->NbRegs=0;
    Song->InPlay=0;
    Song->NumDD=0;
    Song->OldDD=0;
    
    Song->VolA=0;
    Song->VolB=0;
    Song->VolC=0;
    
    memset(Song->OldBuffer[0],0,MAX_BUFFER);
    memset(Song->OldBuffer[1],0,MAX_BUFFER);
    memset(Song->OldBuffer[2],0,MAX_BUFFER);

    memset(Song->NewBuffer[0],0,MAX_BUFFER);
    memset(Song->NewBuffer[1],0,MAX_BUFFER);
    memset(Song->NewBuffer[2],0,MAX_BUFFER);

    if(Song!=NULL) Ret=DecodeHeader(Song);

    if(Ret==1) printf("\nUNKNOWN FILE TYPE\n");

    Song->YM1 = malloc(buffer_size * 2);
    Song->YM2 = malloc(buffer_size * 2);
    Song->YM3 = malloc(buffer_size * 2);
    
    // clear the mem!
    memset(Song->YM1, 0, buffer_size * 2);
    memset(Song->YM2, 0, buffer_size * 2);
    memset(Song->YM3, 0, buffer_size * 2);    
    
    
    for(i=0;i<MAX_DD;i++)
	{
	Song->DigitDrums[i].DataCVT=NULL;
	Song->DigitDrums[i].DataPos=0;
	}
    
    Song->SidSound.Count=0;
    // MFP Init
    MFP_TP[0]=-1;
    MFP_TP[1]=4;
    MFP_TP[2]=10;
    MFP_TP[3]=16;
    MFP_TP[4]=50;
    MFP_TP[5]=64;
    MFP_TP[6]=100;
    MFP_TP[7]=200;
    
    Song->NumChip=NumChip;
    
    return Song;
    }

/*************************
 *                       *
 * Initialise un chip YM *
 *                       *
 *************************/
void InitChip(YMData *Song)
    {
    ///////////////////////////////////////////////////////////////
    // Initialize the AY8910/YM2149 chip
    ///////////////////////////////////////////////////////////////
        
    // initialize chip

    AY8910_init(Song->NumChip, Song->Clock, data_sample_rate, 8);
    // reset chip
    AY8910_reset(Song->NumChip);
    // set output volume of chip
    AY8910_set_volume(Song->NumChip, 255, 0);
    
    printf(" # AY8910/YM2149 Init!\n");
    printf(" # Playing! - Press Any Key To Quit!\n");
    
    ///////////////////////////////////////////////////////////////
    }
    
/**************************************************************
 *                                                            *
 * Decodage de l'entete du fichier et affectation des donnees *
 * dans la structure                                          *
 *                                                            *
 **************************************************************/
int DecodeHeader(YMData *Song)
    {
    int	i,j,k;
    int	Pos=0;
    unsigned char	mydwordstr[4],mywordstr[4];
    unsigned long int	mydwordint;
    unsigned int	mywordint;
        
    
    // Get ID
    for(i=0;i<4;i++)
	Song->ID[i]=Song->FileData[Pos++];
	
    Song->ID[4]=0;
    Song->FileType=-1;
    if(strcmp(Song->ID,"YM2!")==0) {Song->FileType=0;Song->NbRegs=14;}
    if(strcmp(Song->ID,"YM3!")==0) {Song->FileType=1;Song->NbRegs=14;}
    if(strcmp(Song->ID,"YM3b")==0) {Song->FileType=2;Song->NbRegs=14;}
    if(strcmp(Song->ID,"YM5!")==0) {Song->FileType=3;Song->NbRegs=16;}
    if(strcmp(Song->ID,"YM6!")==0) {Song->FileType=4;Song->NbRegs=16;}
    
    //if(strcmp(Song->ID,"MIX1")==0) Song->FileType=5;    
    if(Song->FileType==-1) return 1;
    
    // YM2! / YM3!
    if(Song->FileType==0 || Song->FileType==1)
	{
        Song->YMRegCount = 1;
	Song->StartPos=4;
	Song->Loop=Song->StartPos;
        Song->NBVbl = ((Song->DataFileLength - Song->StartPos) / 14);
        Song->Clock =  2000000;
        Song->YM3bEndOffset = 0;
	FPS=50;
	buffer_size=data_sample_rate/FPS;
	}

    // YM3b
    if(Song->FileType==2)
	{
        Song->YM3bEndOffset = 4;
            for (i=0;i<4;i++)
        	{
                mydwordstr[i]=Song->FileData[(Song->DataFileLength-4)+i];
        	}
            mydwordint = (*((unsigned long int *)mydwordstr));
            Song->Loop=Song->StartPos+mydwordint;
        }
    
        Song->DataPos=Song->StartPos;
	FPS=50;
	buffer_size=data_sample_rate/FPS;
    // **********************************************************************    
    // PaCifiST/STSound "YM5!" and "YM6!" files
    // **********************************************************************
    if (Song->FileType == 3 || Song->FileType == 4)
	{
        Song->YMRegCount = 1;

        // read in header 4 bytes (from data offset 0)

	for (i=0;i<4;i++)
    	    {
            mydwordstr[i]=Song->FileData[i];
    	    }
        mydwordint = htonl(*((unsigned long int *)mydwordstr));

	// Get LeOnArD ( Off 4 )
	for(i=0;i<8;i++)
	    {
	    Song->String[i]=Song->FileData[i+4];
	    }
	Song->String[8]=0;
	
	// Get NBVbl ( OFF 12 )
        for (i=0;i<4;i++)
    	    {
            mydwordstr[i]=Song->FileData[i+12];
    	    }
        mydwordint=htonl(*((unsigned long int *)mydwordstr));
	Song->NBVbl=mydwordint;
	
	// Get Attribut ( Off 16 )
        for (i=0;i<4;i++)
    	    {
            mydwordstr[i]=Song->FileData[i+16];
    	    }
        mydwordint=htonl(*((unsigned long int *)mydwordstr));
	Song->Attribut=mydwordint;
	
	// Get NB_DD ( Off 20 )
	for (i=0;i<2;i++)
	    {
	    mywordstr[i]=Song->FileData[i+20];
	    }
	mywordint=htons(*((unsigned int *)mywordstr));
	Song->NB_DD=mywordint;

        // Get clockrate ( Off 22 )
        for (i=0;i<4;i++)
    	    {
            mydwordstr[i]=Song->FileData[i+22];
    	    }
        mydwordint=htonl(*((unsigned long int *)mydwordstr));
        Song->Clock=mydwordint;

        // Get Freq ( Off 26 )
        for (i=0;i<2;i++)
    	    {
            mywordstr[i]=Song->FileData[i+26];
    	    }
        mywordint=htons(*((unsigned int *)mywordstr));
        Song->Freq=mywordint;
	FPS=Song->Freq;	
	buffer_size=data_sample_rate/FPS;
        // Get AddData ( Off 32 )
        for (i=0;i<2;i++)
    	    {
            mydwordstr[i]=Song->FileData[i+32];
    	    }
        mywordint=htons(*((unsigned long int *)mywordstr));
        Song->AddData=mywordint;
	
        // DataPos=34 don't forget to change if header size change
        Song->DataPos=34;
        Pos = Song->DataPos;

	// Get DD data if exist
        if(Song->NB_DD!=0)
	    {
	    for(j=0;j<Song->NB_DD;j++)
		{
		// Get size of one sample
		for (k=0;k<4;k++)
		    {
        	    mydwordstr[k]=Song->FileData[k+Pos];
    		    }
		mydwordint=htonl(*((unsigned long int *)mydwordstr));
    		Song->DigitDrums[j].Size=mydwordint;
		Pos+=4;
		Song->DigitDrums[j].Data=(unsigned char*)malloc(Song->DigitDrums[j].Size);
		// Get sample data
		for(k=0;k<Song->DigitDrums[j].Size;k++)
		    {
		    if((Song->Attribut & 0x04)==0x04) Song->DigitDrums[j].Data[k]=(Song->FileData[k+Pos] <<4 );
		    else Song->DigitDrums[j].Data[k]=Song->FileData[k+Pos];
		    }
		Pos+=Song->DigitDrums[j].Size;
		}
	    Song->DataPos=Pos;
	    }
	// We are after DD datas

        // read songtitle
        for (i=0;i<MAX_String;i++)
    	    {	
	    Song->Name[i]=Song->FileData[i+Pos];
            Song->DataPos++;
            if (Song->FileData[i+Pos] == 0x00) break;
    	    }
        Pos=Song->DataPos;
        
        // read author
        for (i=0;i<MAX_String;i++)
	    {
            Song->Author[i]=Song->FileData[i+Pos];
            Song->DataPos++;
            if (Song->FileData[i+Pos] == 0x00) break;
	    }
        Pos=Song->DataPos;
        
        // read comments
        for (i=0;i<MAX_String;i++)
    	    {
            Song->Comments[i]=Song->FileData[i+Pos];
            Song->DataPos++;
            if (Song->FileData[i+Pos] == 0x00) break;
    	    }
        
        Song->StartPos=Song->DataPos;
        
        // calculate number of vblanks (ie.. gap in file between successive registers in file)
        //YM_num_VBLs = ((data_filelength - YM_start_pos - 4) / 16);        
        
        // get the fine loop pos
        for (i=0;i<4;i++)
    	    {
    	    mydwordstr[i] = Song->FileData[28+i];
    	    }
        // transfer string into the int (htonl makes sure the hex is of the correct INTEL order)
        mydwordint = htonl(*((unsigned long int *)mydwordstr));
        Song->Loop = Song->StartPos + mydwordint;
        }
    return 0;
    }
    
/********************************
 *                              *
 * Charge la musique en memoire *
 *                              *
 ********************************/
YMData *LoadSong(unsigned char *Name)
    {
    FILE	*Fic;
    YMData	*Song;
    
    Song=(YMData*)malloc(sizeof(struct YMDATA));
    Fic = fopen(Name,"rb");
    if (Fic == NULL)
	{
        printf("ERROR:\t%s FILE DOES NOT EXIST!\n",Name);
	return NULL;
	}
    stat(Name,&Stat);
    Song->DataFileLength = Stat.st_size;

    Song->FileData =(unsigned char *)malloc(Song->DataFileLength);

    fread(Song->FileData,1,Song->DataFileLength,Fic);

    fclose(Fic);

    return Song;
    }

/********************************************************
 *                                                      *
 * MIX un morceau de Digit Drum avec une voie soundchip *
 *                                                      *
 ********************************************************/
int MixDD(YMData *Song)
    {
    unsigned char	NumDD;
    long		DataPos;
    int			InPlay;
    int			i;
    unsigned char	*src,*dest;
    
    if(Song->InPlay==0) return 0;
    else 
	{
	NumDD=Song->NumDD;
    
	// Mix DD whith Buffer
	if(Song->NumVoice==1) dest=Song->YM1;
	if(Song->NumVoice==2) dest=Song->YM2;
	if(Song->NumVoice==3) dest=Song->YM3;

	DataPos=Song->DigitDrums[NumDD].DataPos;

	if(Song->DigitDrums[NumDD].NumBlock < Song->DigitDrums[NumDD].NbBlocks)
	    {
	    for(i=0;i<buffer_size;i++)
		{
		dest[i]=Song->DigitDrums[NumDD].DataCVT[(Song->DigitDrums[NumDD].NumBlock*buffer_size)+i];
		DataPos++;
		}
	    Song->DigitDrums[NumDD].DataPos=DataPos;
	    Song->DigitDrums[NumDD].NumBlock++;
	    Song->InPlay=1;
	    }
	if(Song->DigitDrums[NumDD].NumBlock == Song->DigitDrums[NumDD].NbBlocks)
	    {
	    for(i=0;i<(Song->DigitDrums[NumDD].CvtSize-DataPos);i++)
		{
		dest[i]=Song->DigitDrums[NumDD].DataCVT[i+DataPos];
		}
	    Song->DigitDrums[NumDD].DataPos=0;
	    Song->DigitDrums[NumDD].NumBlock=0;
	    Song->InPlay=0;
	    }
	}
    return InPlay;
    }

/***************************************************
 *                                                 *
 * Converti un sample avec une frequence de depart *
 * en un autre sample avec une frequence plus gde  *
 *                                                 *
 ***************************************************/
void CVTDigitDrums(YMData *Song)
    {
    int			SizeCVT;
    int			Size;
    int			Freq;
    int			NbBlocks;
    
    unsigned char	*src,*dest;
    int			incr;
    int			i,j,k;

    Song->DigitDrums[Song->NumDD].NbBlocks=0;
    Song->DigitDrums[Song->NumDD].NumBlock=0;
    Song->DigitDrums[Song->NumDD].DataPos=0;
    
    if(Song->DigitDrums[Song->NumDD].DataCVT != NULL ) free(Song->DigitDrums[Song->NumDD].DataCVT);
    
    // SizeCVT
    Size=Song->DigitDrums[Song->NumDD].Size;
    Freq=Song->DigitDrums[Song->NumDD].Freq;
    SizeCVT=(Size*data_sample_rate)/Freq;
    Song->DigitDrums[Song->NumDD].CvtSize=SizeCVT;
    Song->DigitDrums[Song->NumDD].DataCVT=(unsigned char *)malloc(SizeCVT);
    
    // NbBlocks
    NbBlocks=SizeCVT/(data_sample_rate/FPS);
    Song->DigitDrums[Song->NumDD].NbBlocks=NbBlocks;
    
    // CVT data -> 44100Hz
    src=Song->DigitDrums[Song->NumDD].Data;
    dest=Song->DigitDrums[Song->NumDD].DataCVT;
    incr=data_sample_rate/Freq;
    k=0;
    for(i=0;i<Size;i++)
	{
	for(j=0;j<incr;j++)
	    {
	    dest[k]=src[i];
	    k++;
	    }
	}
    }
    
/***********************************************
 *                                             *
 * Routine permettant de rejouer le morceau YM *
 *                                             *
 ***********************************************/    
void PlayMusic(YMData *Song)
	{
	static int	IndiceVolumeBuffer=0;
	int register_byte=0;
	int value_byte=0;
	int i;

	//YM2 Specific
	if(Song->FileType==0)
	    {
	    for(Song->YMRegCount=0;Song->YMRegCount<Song->NbRegs;Song->YMRegCount++)
		{
    		register_byte = Song->YMRegCount;
    		value_byte = Song->FileData[Song->DataPos + (Song->YMRegCount * Song->NBVbl)];
		Song->Regs[Song->YMRegCount].Value=value_byte;
		// Register use for waveform	    
    		if (register_byte <11)
        	    {
            	    _AYWriteReg(0, register_byte, value_byte);
    		    }
		else 
	    	    {
		    if(register_byte==13)
			{
			if(value_byte!=0xff)
			    {
			    _AYWriteReg(0, 13, Song->Regs[11].Value);
			    }
			}
	    	    if(value_byte !=0xff && register_byte!=12) _AYWriteReg(0, register_byte, value_byte);
		    }
    		}
	    if(Song->Regs[10].Value & 0x80) //DD
		{
		int Num_DD;
		long Freq_DD;

	    	Song->InPlay=1;
	    	Song->NumVoice=3;

		Num_DD=Song->Regs[10].Value & 0x7f;
		Song->NumDD=Num_DD;
		Freq_DD=(MFP_Frequency/4)/Song->Regs[12].Value;
		
		_AYWriteReg(0, 7, Song->Regs[7].Value | 0x24);

		if(Song->DigitDrums[Num_DD].Data==NULL)
		    {
		    Song->DigitDrums[Num_DD].Data=(unsigned char *)malloc(sampleLen[Num_DD]);
		    memcpy(Song->DigitDrums[Num_DD].Data,sampleAdress[Num_DD],sampleLen[Num_DD]);
		    }
		Song->DigitDrums[Num_DD].Size=sampleLen[Num_DD];
		Song->DigitDrums[Num_DD].NumBlock=0;
		Song->DigitDrums[Num_DD].DataPos=0;

	    	if(Song->DigitDrums[Num_DD].Freq!=Freq_DD) 
	    	    {
		    Song->DigitDrums[Num_DD].Freq=Freq_DD;
		    CVTDigitDrums(Song);
		    }
		}
	    }	
	else
	    {
	    for(Song->YMRegCount=0;Song->YMRegCount<Song->NbRegs;Song->YMRegCount++)
		{
    		register_byte = Song->YMRegCount;
    		value_byte = Song->FileData[Song->DataPos + (Song->YMRegCount * Song->NBVbl)];
		Song->Regs[Song->YMRegCount].Value=value_byte;
		// Register use for waveform	    
    		if (register_byte != 13)
        	    {
            	    _AYWriteReg(0, register_byte, value_byte);
    		    }
		else 
	    	    {
	    	    if(value_byte !=0xff ) _AYWriteReg(0, register_byte, value_byte);
	    	    }	
    		}
	    }	    
	// Update buffers
    	AY8910Update_16(0, Song->YM1, Song->YM2, Song->YM3, buffer_size);
	if (Song->FileType == 0) MixDD(Song);    

	// YM5/YM6 Specific
	if (Song->FileType == 3 || Song->FileType == 4)
	    {
	    //FOR DD
	    int			DD_Freq;
	    unsigned char	DD_TP;
	    unsigned char	DD_TC;
	    unsigned char	NumDD;
	    int			DDPresent;
	    //FOR SID
	    int			SID_Freq;
	    unsigned char	SID_TP;
	    unsigned char	SID_TC;
	    int			SID_Vmax;
	    int			SID_Duration;
	    
	    // Look if there is SID
	    switch(Song->Regs[1].Value & 0x30)
		{
		case 0x30:
	    	    SID_TP=MFP_TP[(Song->Regs[6].Value & 0xe0) >> 5];
	    	    SID_TC=Song->Regs[14].Value;
	    	    if(SID_TC != 0 ) Song->SidSound.Freq=((MFP_Frequency/SID_TP)/SID_TC);
		    SID_Freq=Song->SidSound.Freq;
		    Song->SidSound.VMax=Song->Regs[10].Value & 0x0f;
		    SID_Vmax=Song->SidSound.VMax;
		    Song->SidSound.Duration=data_sample_rate/SID_Freq;
		    SID_Duration=Song->SidSound.Duration;

		    //printf("SID on C : %6d\t%6d\t%6d\n",SID_Freq,SID_Vmax,SID_Duration);
		    break;
		    
		case 0x20:
	    	    SID_TP=MFP_TP[(Song->Regs[6].Value & 0xe0) >> 5];
	    	    SID_TC=Song->Regs[14].Value;
	    	    if(SID_TC != 0 ) Song->SidSound.Freq=((MFP_Frequency/SID_TP)/SID_TC);
		    SID_Freq=Song->SidSound.Freq;
		    Song->SidSound.VMax=Song->Regs[9].Value & 0x0f;
		    SID_Vmax=Song->SidSound.VMax;
		    Song->SidSound.Duration=data_sample_rate/SID_Freq;
		    SID_Duration=Song->SidSound.Duration;

		    //printf("SID on B : %6d\t%6d\t%6d\n",SID_Freq,SID_Vmax,SID_Duration);
		    break;
		    
		case 0x10:
	    	    SID_TP=MFP_TP[(Song->Regs[6].Value & 0xe0) >> 5];
	    	    SID_TC=Song->Regs[14].Value;
    		    if(SID_TC != 0 ) Song->SidSound.Freq=((MFP_Frequency/SID_TP)/SID_TC);
		    SID_Freq=Song->SidSound.Freq;
		    Song->SidSound.VMax=Song->Regs[8].Value & 0x0f;
		    SID_Vmax=Song->SidSound.VMax;
		    Song->SidSound.Duration=data_sample_rate/SID_Freq;
		    SID_Duration=Song->SidSound.Duration;

		    //printf("SID on A : %6d\t%6d\t%6d\n",SID_Freq,SID_Vmax,SID_Duration);
		    break;
		}
	    
	    // Look if we have DD to play
	    switch(Song->Regs[3].Value & 0x30)
		{
		case 0x30:
	    	    Song->InPlay=1;
	    	    Song->NumVoice=3;

	    	    // Get Num DD
	    	    NumDD=(Song->Regs[10].Value & 0x1f);
	    	    Song->NumDD=NumDD;
		
	    	    // Get DD Frequency replay
	    	    DD_TP=MFP_TP[(Song->Regs[8].Value & 0xe0) >> 5];
	    	    DD_TC=Song->Regs[15].Value;
	    	    if(DD_TC != 0 ) DD_Freq=((MFP_Frequency/DD_TP)/DD_TC);
	    	    if(Song->DigitDrums[NumDD].Freq!=DD_Freq) 
	    		{
			Song->DigitDrums[NumDD].Freq=DD_Freq;
			CVTDigitDrums(Song);
			}
		    Song->DigitDrums[NumDD].NumBlock=0;
		    Song->DigitDrums[NumDD].DataPos=0;
		    break;
		case 0x10:
		    Song->InPlay=1;
		    Song->NumVoice=1;

		    // Get Num DD
		    NumDD=(Song->Regs[8].Value & 0x1f);
		    Song->NumDD=NumDD;
		
		    // Get DD Frequency replay
		    DD_TP=MFP_TP[(Song->Regs[8].Value & 0xe0) >> 5];
		    DD_TC=Song->Regs[15].Value;
		    if(DD_TC != 0 ) DD_Freq=((MFP_Frequency/DD_TP)/DD_TC);
		    if(Song->DigitDrums[NumDD].Freq!=DD_Freq) 
		        {
		        Song->DigitDrums[NumDD].Freq=DD_Freq;
		        CVTDigitDrums(Song);
		        }
		    Song->DigitDrums[NumDD].NumBlock=0;
		    Song->DigitDrums[NumDD].DataPos=0;
		    break;
		case 0x20:
		    Song->InPlay=1;
		    Song->NumVoice=2;

		    // Get Num DD
		    NumDD=(Song->Regs[9].Value & 0x1f);
		    Song->NumDD=NumDD;
		
		    // Get DD Frequency replay
		    DD_TP=MFP_TP[(Song->Regs[8].Value & 0xe0) >> 5];
		    DD_TC=Song->Regs[15].Value;
		    if(DD_TC != 0 ) DD_Freq=((MFP_Frequency/DD_TP)/DD_TC);
		    if(Song->DigitDrums[NumDD].Freq!=DD_Freq) 
			{
			Song->DigitDrums[NumDD].Freq=DD_Freq;
			CVTDigitDrums(Song);
			}
		    Song->DigitDrums[NumDD].NumBlock=0;
		    Song->DigitDrums[NumDD].DataPos=0;
		    break;
		}
	    MixDD(Song);
	    }
		
	// Prepare Data buffer for volume level
	if((Song->OldBuffer[0][IndiceVolumeBuffer]=Song->Regs[8].Value&0x1F)>16)Song->OldBuffer[0][IndiceVolumeBuffer]=16;
	if((Song->OldBuffer[1][IndiceVolumeBuffer]=Song->Regs[9].Value&0x1F)>16)Song->OldBuffer[1][IndiceVolumeBuffer]=16;
	if((Song->OldBuffer[2][IndiceVolumeBuffer]=Song->Regs[10].Value&0x1F)>16)Song->OldBuffer[2][IndiceVolumeBuffer]=16;
//	Song->OldBuffer[1][IndiceVolumeBuffer]=Song->Regs[9].Value&0x1F;	
//	Song->OldBuffer[2][IndiceVolumeBuffer]=Song->Regs[10].Value&0x1F;
	
	if(IndiceVolumeBuffer==MAX_BUFFER)
	    {
	    memcpy(Song->NewBuffer[0],Song->OldBuffer[0],MAX_BUFFER);
	    memcpy(Song->NewBuffer[1],Song->OldBuffer[1],MAX_BUFFER);
	    memcpy(Song->NewBuffer[2],Song->OldBuffer[2],MAX_BUFFER);
	    IndiceVolumeBuffer=0;
	    }
	    
	Song->VolA=Song->NewBuffer[0][IndiceVolumeBuffer];
	Song->VolB=Song->NewBuffer[1][IndiceVolumeBuffer];
	Song->VolC=Song->NewBuffer[2][IndiceVolumeBuffer];
	IndiceVolumeBuffer++;

	// Play buffers (AUDIO)
	PlayBuffers(Song);

    	// reset register counter and inc data position by one
    	Song->DataPos++;

	// Loop ?
    	if (Song->DataPos + ((Song->NbRegs-1) * Song->NBVbl) == Song->DataFileLength - 4 && Song->Loopmode == 1)
    	    {
    	    Song->DataPos = Song->Loop;
    	    }
	} 

/********************************
 *                              *
 * Arrete l'emulation soundchip *
 *                              *
 ********************************/	
void CloseChip(YMData *Song)
    {
    AY8910_reset(Song->NumChip);
    }

/*********************************************
 *                                           *
 * Libere la memoire allouee pour la musique *
 *                                           *
 *********************************************/
void ReleaseSong(YMData *Song)
    {
    int	i;
    
    free(Song->YM1);
    free(Song->YM2);
    free(Song->YM3);
    free(Song->FileData);
    
    for(i=0;i<MAX_DD;i++)
	{
	if(Song->DigitDrums[i].Data!=NULL) free(Song->DigitDrums[i].Data);
	if(Song->DigitDrums[i].DataCVT!=NULL) free(Song->DigitDrums[i].DataCVT);
	}
    free(Song);
    }