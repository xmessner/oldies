#include <entete.h>

int main(int argc, char *argv[])
    {           
    YMData	*Song;
    int		sec=0;
    int		VBLLeft=0;
    int		oldA=0,oldB=0,oldC=0;
    int		va=0,vb=0,vc=0;
    int		i;
    
    // no arguments give, so error message
    if (argc == 1)
	{
        printf("\n YMPLAY v0.8");
	printf("\n Linux port by mEEx ( http://www.multimania.fr/mehex" );
        printf("\n AY8910/YM2149 Emulation (C)MAME Team");
        printf("\n Use : ymplay <MUSIC FILE>\n");
        exit(0);
	}
    
    // little messages
    printf("\n # YMPLAY v0.8");
    printf("\n # By mEEx ( http://www.multimania.fr/mehex )" );
    printf("\n # MIDAS / SDL Audio Initialized!\n");    


    // Load and init song definition
    Song=InitSong(argv[1],0);
    Song->Loopmode=1;

    // check for "-loop" argument for looping
    if (argc == 3 && strcmp("-loop", argv[2]) == 0)
	{
        Song->Loopmode = 1;
	}    

    // loop msg       
    if (Song->Loopmode == 1)
	{
        printf(" # LOOP Mode!\n");
	}

    // Init YM2149 Kernel
    InitChip(Song);
        
    // INIT AUDIO
    InitSound(Song);


    /************************
     * Play Soundchip Music *
     ************************/
//    if (Song->FileType == 0 || Song->FileType == 1 || Song->FileType == 2) Song->NbRegs=14;
//    if (Song->FileType == 3 || Song->FileType == 4) Song->NbRegs=16;

    // Printf informations about the song
    PrintInfo(Song);

    VBLLeft=Song->NBVbl;
    while ((Song->DataPos + ((Song->NbRegs-1) * Song->NBVbl)) < Song->DataFileLength - 4 )
    	{                    
	PlayMusic(Song);
	VBLLeft--;
	sec=VBLLeft/FPS;
	if(VBLLeft==0) VBLLeft=Song->NBVbl;
	//printf("\t\033[1;34mTime Left    :\t%02d:%02d\r",sec/60,sec%60);
	printf("A [");
	for(i=0;i<Song->VolA;i++) printf("#");
	for(i=Song->VolA;i<16;i++) printf(" ");
	printf("]");
	printf(" B [");
	for(i=0;i<Song->VolB;i++) printf("#");
	for(i=Song->VolB;i<16;i++) printf(" ");
	printf("]");
	printf(" C [");
	for(i=0;i<Song->VolC;i++) printf("#");
	for(i=Song->VolC;i<16;i++) printf(" ");
	printf("] %02d:%02d\r",sec/60,sec%60);
	Song->VolA=0;Song->VolB=0;Song->VolC=0;
	
	fflush(stdout);
	}
    
    // shutdown eveything and exit
    CloseChip(Song);
    CloseSound();

    ReleaseSong(Song);
    exit(0);
    }

void PrintInfo(YMData *Song)
    {
    int	Sec=Song->NBVbl/FPS;
    printf("\tFile Type    :\t");
    switch(Song->FileType)
	{
	case 0:
	    printf("YM2!\n");
	    break;
	case 1:
	    printf("YM3!\n");
	    break;
	case 2:
	    printf("YM3b\n");
	    break;
	case 3:
	    printf("YM5!\n");
	    break;
	case 4:
	    printf("YM6!\n");
	}
    printf("\tNB Vbl       :\t%ld\n",Song->NBVbl);
    
    if((Song->Attribut & 0x01)==0x01)
	printf("\tInterleaved  :\tYES\n");
    else
	printf("\tInterleaved  :\tNO\n");
	
    if(Song->NB_DD==0)
	printf("\tDD           :\tNO\n");
    if(Song->NB_DD>0)
	{
	printf("\tDD           :\t%d\n",Song->NB_DD);
	if((Song->Attribut & 0x02)==0x02)
	    printf("\tSigned DD    :\tYES\n");
	else
	    printf("\tSigned DD    :\tNO\n");
	
	if((Song->Attribut & 0x04)==0x04)
	    printf("\t4bits DD     :\tYES\n");
	else
	    printf("\t4bits DD     :\tNO\n");
	}
    printf("\tClock rate   :\t%d\n",Song->Clock);
    printf("\tFreq         :\t%d\n",Song->Freq);
    printf("\tLoop         :\t%d\n",Song->Loop);
    printf("\tSong Name    :\t%s\n",Song->Name);
    printf("\tSong Author  :\t%s\n",Song->Author);
    printf("\tSong Name    :\t%s\n",Song->Comments);
    printf("\tTotal Time   :\t%02d:%02d\n",Sec/60,Sec%60);
    }