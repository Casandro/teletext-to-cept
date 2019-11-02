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

//Mosaik is pointer to integer if *mosaik==1 => graphics active, no translation
void print_btx_char(uint8_t d, int lang, int * mosaik)
{
	d=d&0x7f;
	if (d<0x20) {
		printf("%c", d|0x80); 
		if ((d>=0x10) && (d<=0x17)) *mosaik=1;
		if ((d>=0x18) && (d<=0x1F)) *mosaik=0;
	}else
	if (*mosaik==1) printf("%c", d); else
	//#
	if (d==0x23) printf("\x23"); else
	//$
	if (d==0x24) printf("\x19\x34"); else
	//§
	if (d==0x40) printf("\x19\x37"); else
	//Ä
	if (d==0x5b) printf("\x19HA"); else
	//Ö
	if (d==0x5c) printf("\x19HO"); else
	//Ü
	if (d==0x5d) printf("\x19HU"); else
	//^
	if (d==0x5e) printf("\x5e"); else 
	//_
	if (d==0x5f) printf("\x5f"); else 
	//°
	if ( (d==0x60) ) printf("\x19\x30"); else 
	//ä
	if (d==0x7b) printf("\x19Ha"); else
	//ö
	if (d==0x7c) printf("\x19Ho"); else
	//ü
	if (d==0x7d) printf("\x19Hu"); else
	//ß
	if (d==0x7e) printf("\x19\x7b"); else 
	printf("%c",d);
}

void print_line(int row, uint8_t *data, int lang)
{
	if (row>23) return;
	if (row<0) return;
	int mosaik=0;
	int col=0;
	if (row==0) col=8;
	printf("\x87"); //kill features
	printf("\x1f%c%c",0x41+row,0x41+col);
	int n;
	int cnt=0;
	int ac=-1;
	for (n=col; n<40; n++) {
		int c=data[n];
		if ((c!=ac)||(n==39)) {
			if (ac>=0) {
				if (ac<0x20) {
					int m;
					for (m=0; m<cnt; m++) print_btx_char(ac,-1, &mosaik);
				} else
				if (cnt==1) print_btx_char(ac,-1, &mosaik); 
				else {
					int cnt_tmp=cnt;
					while (cnt_tmp>0) {	
						int dec=cnt_tmp;
						if (dec>40) dec=40;
						print_btx_char(ac,-1, &mosaik);
						printf("\x12%c", 0x41+dec-2);
						cnt_tmp=cnt_tmp-dec;
					}
					int m;
					for (m=0; m<cnt_tmp; m++) print_btx_char(ac,-1, &mosaik);
				}
			}
			if (n==39) print_btx_char(c,-1, &mosaik);
			ac=c;
			cnt=1;
		} else {
			cnt=cnt+1; //count repetitions
		}
	}
}



int main(int argc, char *argv[])
{
	printf("\x1F\x2D\x71");
	printf("\x1F\x2F\x41"); //reset terminal to serial attributes
	int lang=0;
	uint8_t buf[PLEN];
	while (read(0, buf, PLEN)==PLEN) {
		int mpag=de_hamm(buf[1])<<4 | de_hamm(buf[0]);
		int magazine=mpag&0x7;
		int row=mpag>>3;
		if (row==0) {
			int page=de_hamm(buf[2])<<4 | de_hamm(buf[3]);
			int sub=(de_hamm(buf[4])) | (de_hamm(buf[5])<<4) | (de_hamm(buf[6])<<8) | (de_hamm(buf[7])<<12);
			int subpage=sub&0x3f7f;
			lang=(de_hamm(buf[7])>>1); //Language 000-111
		}
		if ( (row>=0) && (row<=23) ) print_line(row, &(buf[2]), lang);
	}
}
