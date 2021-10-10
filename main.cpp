#include "main.h"
#include "bmp_io.h"

#define BLOCKW 8
#define BLOCKH 8
#define IMGPIXELH 512
#define IMGPIXELW 512

void data_embedding(int);
void data_recovery(int);
int average_function(unsigned char *, int *, int, int);
int side_matching_function(unsigned char *, int *, int, int);
int up_down_matching_function(unsigned char *, int *, int, int);

double abs_f(double );
int get_rand(int, int);
int inversion(int );
int *get_new_key_code();
int *get_key_code();
int *get_data();


int main(int argc, char *argv[])
{
	int whole_mode, key_mode, recovery_mode;
	printf("DATA hiding and recover to img made by S.D.M\n");
	printf("Select whole mode(Hiding 1, Recovery 2, Exit 0) : ");
	scanf("%d", &whole_mode);
	srand((unsigned)time(NULL));
	if(whole_mode == 0) return 0;
	
	else if(whole_mode == 1)
	{
		printf("Select key mode(Make 1, Road 2, Exit 0) : ");
		scanf("%d", &key_mode);	
		if(key_mode == 0) return 0;
		data_embedding(key_mode);
	}
	
	else if(whole_mode == 2)
	{
		printf("Select recovery mode(Average 1, Side 2, Up and Down 3) : ");
		scanf("%d", &recovery_mode);
		if(recovery_mode == 0) return 0;
		data_recovery(recovery_mode);
	}
	
	else
	{
		printf("error!");
		return 0;
	}
	
	return 0;
}

void data_embedding(int key_mode)
{
	
	int *key_code, *data, i, j, bi, bj, cnt;
	bool error;
	long int height;
	unsigned long int width;
	unsigned char *img, *afterimg;
	
	if(key_mode == 1)
	{
		key_code = get_new_key_code();
	}
	
	else if(key_mode == 2)
	{
		key_code = get_key_code();
	}
	
	printf("get key code succeed\n\n");
	printf("Key code is\n\n");
	
	for(i=0; i<BLOCKH; i++)
	{
		for(j=0; j<BLOCKW; j++)
		{
			printf("%d ", key_code[i * BLOCKW + j]);
		}
		printf("\n");
	}
	
	data = get_data();
	
	printf("\n\nget data succeed\n\n");
	printf("data is\n\n");
	
	for(i=0; i<IMGPIXELH * IMGPIXELW / BLOCKW / BLOCKH; i++)
	{
		printf("%d ", data[i]);
	}
	
	///////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////
	
	error = bmp_read("lena.bmp", &width, &height, &img, &img, &img);
	
	///////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////
	
	printf("\n\nget img succed\n\n");
	
	printf("%d %d %03d", width, height, img[0]);
	
	printf("\n\nhiding...\n\n");
	
	afterimg = (unsigned char *)malloc(width * height * sizeof (unsigned char));
	
	for (i = 0; i<height-7; i += BLOCKH)
	{
		for (j = 0; j<width-7; j += BLOCKW)
		{
			for (bi = 0; bi<BLOCKH; bi++)
			{
				for (bj = 0; bj<BLOCKW; bj++)
				{
					cnt = (i + bi)*width + j + bj;
					if (key_code[bi*BLOCKW + bj] == data[i/BLOCKH*width/BLOCKW+j/BLOCKW])
					{
						afterimg[cnt] = inversion(img[cnt]);
					}
					else
						afterimg[cnt] = img[cnt];
				}
			}
		}
	}
	
	printf("hiding succed\n\n");
	
	error = bmp_24_write("inversion.bmp", width, height, afterimg, afterimg, afterimg);

	printf("File saving succed\n\n");
}

int *get_new_key_code()
{
	FILE *key_code_file;
	key_code_file = fopen("key code.txt", "wt");
	int *key_code, i, j, x, y;
	
	key_code = (int *)malloc(BLOCKW * BLOCKH * sizeof (int));
	
	for (i = 0; i < BLOCKH; i++)
	{
		for (j = 0; j < BLOCKW; j++)
		{
			key_code[i * BLOCKW + j] = 0;
		}
	}
	
	for(i=0; i< BLOCKW*BLOCKH/2; i)
	{
		x = get_rand(0, 7);
		y = get_rand(0, 7);
		
		if(key_code[x * BLOCKW + y] == 0)
		{
			key_code[x * BLOCKW + y] = 1;
			i++;
		}
	}
	
	for(i=0; i<BLOCKH; i++)
	{
		for(j=0; j<BLOCKW; j++)
		{
			fprintf(key_code_file, "%d ", key_code[i * BLOCKW + j]);
		}
		fprintf(key_code_file, "\n");
	}
	
	return key_code;
}

void data_recovery(int recovery_mode)
{
	int *key_code, *data, i, j;
	bool error;
	long int height;
	unsigned long int width;
	unsigned char *img, *afterimg;
	
	data = (int *)malloc(IMGPIXELH * IMGPIXELW / BLOCKW / BLOCKH * sizeof (int));
	
	FILE *data_file;
	data_file = fopen("recovery_data.txt", "wt");
	
	key_code = get_key_code();
	
	printf("get key code succeed\n\n");
	printf("Key code is\n\n");
	
	for(i=0; i<BLOCKH; i++)
	{
		for(j=0; j<BLOCKW; j++)
		{
			printf("%d ", key_code[i * BLOCKW + j]);
		}
		printf("\n");
	}
	
	error = bmp_read("send.bmp", &width, &height, &img, &img, &img);
	
	printf("\n\nget img succeed\n\n");
	
	printf("%d %d %03d", width, height, img[0]);
	
	if(recovery_mode == 1)
	{
		for(i=0; i<IMGPIXELH/BLOCKH; i++)
		{
			for(j=0; j<IMGPIXELW/BLOCKW; j++)
			{
				data[i*IMGPIXELW/BLOCKW + j] = average_function(img, key_code, i, j);
			}
		}	
	}
	
	else if(recovery_mode == 2)
	{
		for(i=0; i<IMGPIXELH/BLOCKH; i++)
		{
			for(j=0; j<IMGPIXELW/BLOCKW; j++)
			{
				data[i*IMGPIXELW/BLOCKW + j] = side_matching_function(img, key_code, i, j);
			}
		}	
	}
	
	else if(recovery_mode == 3)
	{
		for(i=0; i<IMGPIXELH/BLOCKH; i++)
		{
			for(j=0; j<IMGPIXELW/BLOCKW; j++)
			{
				data[i*IMGPIXELW/BLOCKW + j] = side_matching_function(img, key_code, i, j);
			}
		}	
	}
	else
	{
		printf("error!");
		return ;
	}
	
	printf("\n\nfunction succeed\n\n");
	
	for(i=0; i<IMGPIXELH*IMGPIXELW/BLOCKH/BLOCKW; i++)
	{
		printf("%d", data[i]);
		fprintf(data_file, "%d", data[i]);
	}
}

int side_matching_function(unsigned char *img, int *key_code, int a, int b)
{
	int i, j, cnt;
	float s1=0.00, s0=0.00;
	unsigned char aft_pix_0[BLOCKW*BLOCKH], aft_pix_1[BLOCKW*BLOCKH];
	
	for(i=0; i<BLOCKH; i++)
	{
		for(j=0; j<BLOCKW; j++)
		{	
		
			cnt = (b*BLOCKH+i)*IMGPIXELW+(a*BLOCKW+j);
			
			if(key_code[i*BLOCKW+j] == 0)
			{
				aft_pix_0[i*BLOCKW+j] = inversion(img[cnt]);
			}
			
			else
			{
				aft_pix_0[i*BLOCKW+j] = img[cnt];
			}
			
			if(key_code[i*BLOCKW+j] == 1)
			{
				aft_pix_1[i*BLOCKW+j] = inversion(img[cnt]);
			}
			
			else
			{
				aft_pix_1[i*BLOCKW+j] = img[cnt];
			}
		}
	}
	
	for(i=1; i<BLOCKH; i++)
	{
		for(j=1; j<BLOCKW-1; j++)
		{
			s1 = s1 + abs_f(aft_pix_1[i*BLOCKW+j]-aft_pix_1[i*BLOCKW+j+1]); 
			s0 = s0 + abs_f(aft_pix_0[i*BLOCKW+j]-aft_pix_0[i*BLOCKW+j+1]); 
		}
	}
	
	for(i=1; i<BLOCKH-1; i++)
	{
		for(j=1; j<BLOCKW; j++)
		{
			s1 = s1 + abs_f(aft_pix_1[i*BLOCKW+j]-aft_pix_1[(i+1)*BLOCKW+j]); 
			s0 = s0 + abs_f(aft_pix_0[i*BLOCKW+j]-aft_pix_0[(i+1)*BLOCKW+j]); 
		}
	}
	
	if(s1>s0) return 0;
	else return 1;
	
}

int average_function(unsigned char *img, int *key_code, int a, int b)
{
	int i, j, cnt;
	float s1 = 0.00, s0 = 0.00;
	unsigned char aft_pix_0[BLOCKW*BLOCKH], aft_pix_1[BLOCKW*BLOCKH];
	
	for(i=0; i<BLOCKH; i++)
	{
		for(j=0; j<BLOCKW; j++)
		{	
			
			cnt = (b*BLOCKH+i)*IMGPIXELW+(a*BLOCKW+j);
			
			if(key_code[i*BLOCKW+j] == 0)
			{
				aft_pix_0[i*BLOCKW+j] = inversion(img[cnt]);
			}
			
			else
			{
				aft_pix_0[i*BLOCKW+j] = img[cnt];
			}
			
			if(key_code[i*BLOCKW+j] == 1)
			{
				aft_pix_1[i*BLOCKW+j] = inversion(img[cnt]);
			}
			
			else
			{
				aft_pix_1[i*BLOCKW+j] = img[cnt];
			}
		}
	}
	
	for(i=1; i<BLOCKH-1; i++)
	{
		for(j=1; j<BLOCKW-1; j++)
		{
			s1 = s1 + abs_f(aft_pix_1[i*BLOCKW+j] - (aft_pix_1[(i+1)*BLOCKW+j]+aft_pix_1[(i-1)*BLOCKW+j]+aft_pix_1[i*BLOCKW+j+1]+aft_pix_1[i*BLOCKW+j-1])/4.00);
			s0 = s0 + abs_f(aft_pix_0[i*BLOCKW+j] - (aft_pix_0[(i+1)*BLOCKW+j]+aft_pix_0[(i-1)*BLOCKW+j]+aft_pix_0[i*BLOCKW+j+1]+aft_pix_0[i*BLOCKW+j-1])/4.00);
		}
	}
	
	if(s1>s0) return 0;
	else return 1;
}

int up_down_matching_function(unsigned char *img, int *key_code, int a, int b)
{
	int i, j, cnt;
	float s1 = 0.00, s0 = 0.00;
	unsigned char aft_pix_0[BLOCKW*BLOCKH], aft_pix_1[BLOCKW*BLOCKH];
	
	for(i=0; i<BLOCKH; i++)
	{
		for(j=0; j<BLOCKW; j++)
		{	
			
			cnt = (b*BLOCKH+i)*IMGPIXELW+(a*BLOCKW+j);
			
			if(key_code[i*BLOCKW+j] == 0)
			{
				aft_pix_0[i*BLOCKW+j] = inversion(img[cnt]);
			}
			
			else
			{
				aft_pix_0[i*BLOCKW+j] = img[cnt];
			}
			
			if(key_code[i*BLOCKW+j] == 1)
			{
				aft_pix_1[i*BLOCKW+j] = inversion(img[cnt]);
			}
			
			else
			{
				aft_pix_1[i*BLOCKW+j] = img[cnt];
			}
		}
	}
	
	for(i=2; i<BLOCKH-1; i++)
	{
		for(j=1; j<BLOCKW; j++)
		{
			s1 = s1 + abs_f(aft_pix_1[i*BLOCKW+j] - (aft_pix_1[(i+1)*BLOCKW+j] + aft_pix_1[(i-1)*BLOCKW+j])/2);
			s0 = s0 + abs_f(aft_pix_1[i*BLOCKW+j] - (aft_pix_0[(i+1)*BLOCKW+j] + aft_pix_0[(i-1)*BLOCKW+j])/2);
		}
	}
	
	for(i=1; i<BLOCKH; i++)
	{
		for(j=2; j<BLOCKW-1; j++)
		{
			s1 = s1 + abs_f(aft_pix_1[i*BLOCKW+j] - (aft_pix_1[i*BLOCKW+j+1] + aft_pix_1[i*BLOCKW+j-1])/2);
			s0 = s0 + abs_f(aft_pix_1[i*BLOCKW+j] - (aft_pix_0[i*BLOCKW+j+1] + aft_pix_0[i*BLOCKW+j-1])/2);
		}
	}
	
	if(s1>s0) return 0;
	else return 1;
}

int *get_key_code()
{
	int i, j, *key_code;
	key_code = (int *)malloc(BLOCKW * BLOCKH * sizeof (int));
	
	FILE *key_code_file;
	key_code_file = fopen("key code.txt", "rt");
	
	for(i=0; i<BLOCKH * BLOCKW; i++)
	{
		fscanf(key_code_file, "%d", &key_code[i]);
 	}
	
	return key_code;
}

int *get_data()
{
	int i, j, *data;
	
	data = (int *)malloc(IMGPIXELH * IMGPIXELW / BLOCKW / BLOCKH * sizeof (int));
	
	FILE *data_file;
	data_file = fopen("data.txt", "rt");
	
	for(i=0; i<IMGPIXELH * IMGPIXELW / BLOCKW / BLOCKH; i++)
	{
		fscanf(data_file, "%1d", &data[i]);
 	}
 	
 	return data;
}

int get_rand(int min, int max)
{
	return rand() % (max-min+1) + min;
}

double abs_f(double a)
{
	if(a<0) return -a;
	return a;
}

int inversion(int x)
{
	return x + 7 - 2 * (x % 8);
}
