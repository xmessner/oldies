#include </usr/local/include/libGFX.h>

int main(int argc,char **argv)
    {
    unsigned char car=0,i;
    char *ptr;
    
    GFXInit(10,"TEST EVENT");
    
    while(car != 1)
	{
	process_events();
	car=__keystate[SDLK_TAB];
	if (car==1) fprintf(stderr,"CARRRRRRR");
	}
    return 0;
    }	