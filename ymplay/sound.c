#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <linux/soundcard.h>

#include "entete.h"

static int audio_dev;

void InitSound(YMData *Song)
    {
    int fd = -1;

    int format = AFMT_U8;
    int speed = 44100;
    int stereo = 0;
    
    fd = open("/dev/dsp", O_WRONLY);

    ioctl(fd, SNDCTL_DSP_RESET, 0);
    ioctl(fd, SNDCTL_DSP_SETFMT, &format);
    ioctl(fd, SNDCTL_DSP_SPEED, &speed);
    ioctl(fd, SNDCTL_DSP_STEREO, &stereo);

    if(fd < 0)
	{
	fprintf(stderr, "Error opening audio.\n");
	exit(-33);
	}
    
    audio_dev = fd;
    // INIT MIDAS ( AUDIO )
//    if(!MIDASstartup()) MIDASerror();
//    MIDASsetOption(MIDAS_OPTION_MIXBUFLEN,150);
//    MIDASsetOption(MIDAS_OPTION_MIXBUFBLOCKS,4);
//    if(!MIDASinit()) MIDASerror();
//    if(!MIDASstartBackgroundPlay(100)) MIDASerror();
//    if(!MIDASopenChannels(NUMSTREAMCHANNELS)) MIDASerror();
//
//    // INIT STRUCT (YMCORE)
//    Song->streamYM1=MIDASplayStreamPolling(MIDAS_SAMPLE_8BIT_MONO,data_sample_rate,buffer_size * 2);
//    Song->streamYM2=MIDASplayStreamPolling(MIDAS_SAMPLE_8BIT_MONO,data_sample_rate,buffer_size * 2);    
//    Song->streamYM3=MIDASplayStreamPolling(MIDAS_SAMPLE_8BIT_MONO,data_sample_rate,buffer_size * 2);
//    
//    // YMCORE
//    MIDASsetStreamVolume(Song->streamYM1,60);
//    MIDASsetStreamVolume(Song->streamYM2,60);
//    MIDASsetStreamVolume(Song->streamYM3,60);*/
    }
    
void PlayBuffers(YMData *Song)
    {
    int i;
//    MIDASfeedStreamData(Song->streamYM1,Song->YM1,buffer_size,1);
//    MIDASfeedStreamData(Song->streamYM2,Song->YM2,buffer_size,1);
//    MIDASfeedStreamData(Song->streamYM3,Song->YM3,buffer_size,1);
    for(i=0;i<buffer_size;i++)
	{
	Song->YM1[i]=Song->YM1[i]>>2;
	if((Song->YM1[i]+(Song->YM2[i]>>2)+(Song->YM3[i]>>2))>255)
	    Song->YM1[i]=255;
	else
	    Song->YM1[i]=Song->YM1[i]+(Song->YM2[i]>>2)+(Song->YM3[i]>>2);
	}
    write(audio_dev, Song->YM1, buffer_size);
    }

void CloseSound(void)
    {
    //MIDASclose();
    close(audio_dev);
    }
    
//void MIDASerror(void)
//    {
//    int	error;
//    
//    error=MIDASgetLastError();
//    printf("\nMIDAS error %s\n",MIDASgetErrorMessage(error));
//    if(!MIDASclose())
//	{
//	printf("\nBIG PROBLEM ON EXIT : %s\n",MIDASgetErrorMessage(MIDASgetLastError()));
//	}
//    exit(EXIT_FAILURE);
//    }