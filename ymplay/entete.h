#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include </usr/include/SDL/SDL.h>
//#include <midasdll.h>
#include "ay8910.h"

#define MAX_String		100
#define MAX_BUFFER		25
#define data_sample_rate	44100
#define MFP_Frequency		2457600
//#define	NUMSTREAMCHANNELS	3

#define MAX_DD	40

typedef struct SID_SOUND {
			    int			Count;
			    int			Duration;
			    int			Freq;
			    int			VMax;
			    int			TVMax;
			    }SID;
			    
typedef struct DIGIT_DRUMS {
			    long		Size;
			    long		CvtSize;
			    long		Freq;
			    long		DataPos;
			    unsigned char	*Data;
			    unsigned char	*DataCVT;
			    int			NbBlocks;
			    int			NumBlock;
			    }DD;
			    
typedef struct YM_REGS {
			int	Value;
			}YmRegs;
			
typedef struct YMDATA {
			unsigned char		ID[5];			// 0  . 4
			unsigned char		String[9];		// 4  . 8
			long			NBVbl;			// 12 . 4
			long			Attribut;		// 16 . 4
			int			NB_DD;			// 20 . 2
			long			Clock;			// 22 . 4
			int			Freq;			// 26 . 2
			long			Loop;			// 28 . 4
			int			AddData;		// 32 . 2
			DD			DigitDrums[MAX_DD];		// 32 DD Max
			SID			SidSound;
			unsigned char		Name[MAX_String];	// Song Name
			unsigned char		Author[MAX_String];	// Author Name
			unsigned char		Comments[MAX_String];	// Comments
			YmRegs			Regs[16];		// Data

			// Other datas
			long			StartPos;
			int			FileType;		// -1 = UNKNOWN
									//  0  = PaCifiST/STSound "YM2!"
									//  1  = PaCifiST/STSound "YM3!"
                                    					//  2  = PaCifiST/STSound "YM3b"
				                                        //  3  = PaCifiST/STSound "YM5!"
				                                        //  4  = PaCifiST/STSound "YM6!"
			long			DataFileLength;
			long			DataPos;
			unsigned char		*YM1,*YM2,*YM3;
			//MIDASstreamHandle	streamYM1,streamYM2,streamYM3;

			long			YM3bEndOffset;
			unsigned char		*FileData;
			long			YMRegCount;
			int			RegisterByte;
			int			ValueByte;
			int			InPlay;
			int			NumDD;
			int			OldDD;
			int			NumVoice;
			int			NbRegs;
			int			NumChip;
			int			Loopmode;
			unsigned char		OldBuffer[3][MAX_BUFFER];
			unsigned char		NewBuffer[3][MAX_BUFFER];
			int			VolA,VolB,VolC;
			}YMData;
			

char MFP_TP[8]; //MFP Values

void _AYWriteReg(int n,int r,int v);
void AY8910Update_16(int chip,unsigned char *buffer_1,unsigned char *buffer_2,unsigned char *buffer_3,int length);
int AY8910_init(int chip,int clock,int sample_rate,int sample_bits);

YMData *LoadSong(unsigned char *Name);
YMData *InitSong(unsigned char *Name,int NumChip);
void PrintInfo(YMData *Song);
int DecodeHeader(YMData *Song);
void CVTDigitDrums(YMData *Song);
int MixDD(YMData *Song);
void PlayMusic(YMData *Song);
void InitSound(YMData *Song);
void InitChip(YMData *Song);
void CloseChip(YMData *Song);
void CloseSound(void);
void PlayBuffers(YMData *Song);
void ReleaseSong(YMData *Song);

struct	stat Stat;
int FPS,buffer_size;
//static	MIDASstreamHandle streamYM1,streamYM2,streamYM3;
//void	MIDASerror(void);