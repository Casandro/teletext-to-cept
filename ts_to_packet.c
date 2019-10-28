#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

#define PLEN (188)


uint8_t reverse_bits(uint8_t x)
{
	int n;
	uint8_t byte=0;
	for (n=0; n<8; n++) {
		uint8_t b=(x>>n)&0x1;
		byte=(byte<<1)|b;
	}
	return byte;
}

#define BIT(x, y) ((x>>y)&0x1)

int de_hamm(uint8_t x)
{
	x=reverse_bits(x);
	return BIT(x,1) | (BIT(x,3)<<1) | (BIT(x,5)<<2) | (BIT(x,7)<<3);
}

int main(int argc, char **argv)
{
	uint8_t buf[PLEN];
	while (read(0, buf, PLEN)==PLEN) {
		uint16_t pid=buf[1];
		pid=((pid<<8) | buf[2])& 0x1fff;
		int n;
		if (/*(pid==wanted_pid)&&*/(buf[6]!=0xff)) {
			int line;
			for (line=0; line<4; line++) {
				int start=4+line*46;
				if (buf[start]==0xff) continue;
				if (buf[start+4]==0xff) continue;
				int mpag=de_hamm(buf[start+4]) | de_hamm(buf[start+5])<<4;
				uint8_t tpacket[41];
				tpacket[0]=mpag;
				for (n=0; n<40; n++) {
					tpacket[n+1]=reverse_bits(buf[start+n+6]);
				}
				write(1, tpacket, sizeof(tpacket));
			}
		}
	}	
}
