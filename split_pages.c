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
	if (argc!=2) {
		printf("Usage: %s <file-prefix>\n", argv[0]);
		return 0;
	}
	char *prefix=argv[1];
	size_t fn_len=strlen(argv[1])+16;
	char *fn=malloc(fn_len);
	
	FILE *f=NULL;

	uint8_t buf[PLEN];
	while (read(0, buf, PLEN)==PLEN) {
		int mpag=de_hamm(buf[1])<<4 | de_hamm(buf[0]);
		int magazine=mpag&0x7;
		int row=mpag>>3;
		if (row==0) {
			int page=de_hamm(buf[3])<<4 | de_hamm(buf[2]);
			int sub=(de_hamm(buf[4])) | (de_hamm(buf[6])<<4) | (de_hamm(buf[6])<<8) | (de_hamm(buf[7])<<12);
			int subpage=sub&0x3f7f;
			snprintf(fn, fn_len, "%s%02x-%04x", prefix, page, subpage);
			if (f!=NULL) {
				fclose(f);
				f=NULL;
			}
			if (page!=0xff) f=fopen(fn, "w");
			if (page!=0xff) {
				printf("Page: %s ", fn);
				int n;
				for (n=10; n<42; n++) {
					uint8_t b=buf[n]&0x7f;
					if (b<0x20) printf(" "); else printf("%c", b);
				}
				printf("\n");
			}
		}
		if (f!=NULL) fwrite(buf, 1, sizeof(buf),f);
	}
	free(fn);
	if (f!=NULL) fclose(f);
}
