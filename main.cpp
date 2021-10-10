#include "main.h"
#include "bmp_io.h"

int main()
{
	FILE *ori_data, *rec_data;
	int i, j, cnt=0, origin_data[4096]={0}, recover_data[4096]={0};
	bool error;
	unsigned char *afterimg;
	
	afterimg = (unsigned char *)malloc(64 * 64 * sizeof (unsigned char));
	
	ori_data = fopen("data.txt","rt");
	rec_data = fopen("recovery_data.txt", "rt");
	
	for(i=0; i<4096; i++)
	{
		fscanf(ori_data, "%1d", &origin_data[i]);
		fscanf(rec_data, "%1d", &recover_data[i]);
	}
	
	for(i=0; i<4096; i++)
	{
		printf("%d", origin_data[i]);
		
		if(origin_data[i] == recover_data[i])
		{
		afterimg[i] = 255;
		cnt++;	
		}
		
		else
		{
		afterimg[i] = 0;
		}
		
	}
	
	error = bmp_24_write("inversion.bmp", 64, 64, afterimg, afterimg, afterimg);
	
	printf("\n\n%f", cnt/4096.00*100);
	while(1);
}
