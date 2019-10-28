#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<strings.h>

#define PLEN 41

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
		int magazine=buf[0]&0x7;
		fwrite(buf, 1, sizeof(buf),f[magazine]);
	}

	for (n=0; n<8; n++) fclose(f[n]);
}
