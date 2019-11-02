#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<strings.h>

#define PLEN 46

#define BIT(x, y) ((x>>y)&0x1)
int de_hamm(uint8_t x)
{
	return BIT(x,1) | (BIT(x,3)<<1) | (BIT(x,5)<<2) | (BIT(x,7)<<3);
}


uint8_t rev(uint8_t b)
{
	return  BIT(b,0)<<7 | BIT(b,1)<<6 | BIT (b,2)<<5 | BIT(b,3)<<4 |
		BIT(b,4)<<3 | BIT(b,5)<<2 | BIT (b,6)<<1 | BIT(b,7);
}

int main(int argc, char *argv[])
{
	if (argc!=2) {
		printf("Usage: %s <file-prefix>\n", argv[0]);
		return 0;
	}
	FILE *f[8];
	int n;
	size_t fn_len=strlen(argv[1])+1;
	char *fn=malloc(fn_len+2);
	strcpy(fn, argv[1]);
	for (n=0; n<8; n++) {
		int md=n;
		if (n==0) md=8;
		fn[fn_len-1]='0'+md;
		fn[fn_len]=0;
		f[n]=fopen(fn, "w");
	}
	free(fn);

	uint8_t buf[PLEN];
	while (read(0, buf, PLEN)==PLEN) {
		uint8_t data_unit_id=buf[0];
		uint8_t data_unit_len=buf[1];
		if ((data_unit_id!=0x02) && (data_unit_id!=0x03)) continue;
		if (data_unit_len!=0x2c) continue;

		uint8_t line=buf[2];
		uint8_t framing_code=buf[3];
		
		uint8_t packet[42];
		for (n=0; n<42; n++) {
			packet[n]=rev(buf[n+4]);
		}
		int mpag=de_hamm(packet[1])<<4 | de_hamm(packet[0]);
		int magazine=mpag&0x7;
		int row=mpag>>3;
		fwrite(packet, 1, sizeof(packet),f[magazine]);
	}

	for (n=0; n<8; n++) fclose(f[n]);
}
