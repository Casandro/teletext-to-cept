#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<strings.h>

#define PLEN 41

#define BIT(x, y) ((x>>y)&0x1)
int de_hamm(uint8_t x)
{
	return BIT(x,1) | (BIT(x,3)<<1) | (BIT(x,5)<<2) | (BIT(x,7)<<3);
}

void print_btx_char(uint8_t d, int lang)
{
	d=d&0x7f;
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
	if ( (d==0x60) && (lang!=-1)) printf("\x19\x30"); else 
	//ä
	if (d==0x7b) printf("\x19Ha"); else
	//ö
	if (d==0x7c) printf("\x19Ho"); else
	//ü
	if (d==0x7d) printf("\x19Hu"); else
	//ß
	if (d==0x7e) printf("\x1d;"); else 
	if (d<0x20) printf("%c",d+0x80) ;
	else printf("%c",d);
}

void print_line(int row, uint8_t *data, int lang)
{
	int col=0;
	if (row==0) col=8;
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
					for (m=0; m<cnt; m++) print_btx_char(ac,-1);
				} else
				if (cnt==1) print_btx_char(ac,-1); 
				else {
					int cnt_tmp=cnt;
					while (cnt_tmp>0) {	
						int dec=cnt_tmp;
						if (dec>40) dec=40;
						print_btx_char(ac,-1);
						printf("\x12%c", 0x41+dec-2);
						cnt_tmp=cnt_tmp-dec;
					}
					int m;
					for (m=0; m<cnt_tmp; m++) print_btx_char(ac,-1);
				}
			}
			if (n==39) print_btx_char(c,-1);
			ac=c;
			cnt=1;
		} else {
			cnt=cnt+1; //count repetitions
		}
	}
	printf("\x87"); //kill features
}



int main(int argc, char *argv[])
{
	int lang=0;
	uint8_t buf[PLEN];
	while (read(0, buf, PLEN)==PLEN) {
		int magazine=buf[0]&0x7;
		int row=buf[0]>>3;
		if (row==0) {
			int page=de_hamm(buf[1])<<4 | de_hamm(buf[2]);
			int sub=(de_hamm(buf[3])) | (de_hamm(buf[4])<<4) | (de_hamm(buf[5])<<8) | (de_hamm(buf[6])<<12);
			int subpage=sub&0x3f7f;
			lang=(de_hamm(buf[7])>>1); //Language 000-111
			printf("\x1F\x2F\x41");
		}
		if ( (row>=0) && (row<=23) ) print_line(row, &(buf[1]), lang);
	}
}
