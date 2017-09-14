#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv)
	{
	FILE *src,*dest;
	unsigned char carlu;

	src=fopen(argv[1],"rb");
	dest=fopen(argv[2],"wb");

	while(!feof(src))
		{
		fread(&carlu,1,1,src);
		if(carlu!=0x0d)
			{
			fwrite(&carlu,1,1,dest);
			}
		}
	return 0;
	}
