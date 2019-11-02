#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<strings.h>

#define PLEN 42

#define BIT(x, y) ((x>>y)&0x1)
int de_hamm(uint8_t x)
{
	return BIT(x,1) | (BIT(x,3)<<1) | (BIT(x,5)<<2) | (BIT(x,7)<<3);
}


int main(int argc, char *argv[])
{

	uint8_t level1[25][40];
	memset(level1, sizeof(level1), 0x20);

	uint8_t buf[PLEN];
	while (read(0, buf, PLEN)==PLEN) {
		int mpag=de_hamm(buf[1])<<4 | de_hamm(buf[0]);
		int magazine=mpag&0x7;
		int row=mpag>>3;
		if ((row>=0) && (row<25)) {
			int n;
			for (n=0; n<40; n++) level1[row][n]=buf[2+n]&0x7f;
		}
	}
	write(1, level1, sizeof(level1));
}
