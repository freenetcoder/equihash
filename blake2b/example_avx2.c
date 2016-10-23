#include <stdio.h>
#include <stdint.h>
#include <string.h>

void Blake2PrepareMidstate4(void *midstate, unsigned char *input);
//midstate: 256 bytes of buffer for output midstate, aligned by 32
//input: 140 bytes header, preferably aligned by 8

void Blake2Run4(unsigned char *hashout, void *midstate, uint32_t indexctr);
//hashout: hash output buffer: 4*64 bytes
//midstate: 256 bytes from Blake2PrepareMidstate4
//indexctr: For n=200, k=9: {0, 4, 8, ..., 1048572}

unsigned char __attribute__((aligned(8))) testdata[140] =
{
    0x04, 0x00, 0x00, 0x00, 0x91, 0x5F, 0xA6, 0x1C, 0x4F, 0xA5, 0x92, 0x3C, 0xE6, 0xEE, 0xAD, 0x06, 
    0x74, 0x6B, 0x61, 0x22, 0x54, 0x94, 0xEA, 0x5A, 0x2A, 0x97, 0xAE, 0x46, 0x6E, 0x6F, 0xAA, 0x9C, 
    0x6E, 0xF6, 0x3A, 0x0D, 0xA5, 0xFC, 0x67, 0xD7, 0xF8, 0xDC, 0x78, 0xC3, 0xC8, 0x70, 0xCA, 0x09, 
    0xBA, 0xAB, 0xAA, 0xF7, 0x02, 0x59, 0x68, 0xA8, 0x6F, 0xEB, 0x88, 0x75, 0xD3, 0xF3, 0xFF, 0xA7, 
    0x2E, 0xB0, 0x0F, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x66, 0xCE, 0xD2, 0x57, 0x0F, 0x0F, 0x0F, 0x20, 0x00, 0x00, 0xF7, 0xF1, 
    0x94, 0xA2, 0x53, 0x8E, 0x42, 0x5F, 0x21, 0x33, 0xCF, 0xA8, 0xD3, 0xCB, 0xF4, 0xDF, 0x71, 0xEF, 
    0x38, 0x28, 0x51, 0x75, 0xCF, 0xED, 0xCB, 0x3E, 0x63, 0xA2, 0x00, 0x00
};
//expected output: 281dd5fc6d878538e640987b9bc597dbbd4af2cdf8bf5fb03bdfcefa40d8747d  out.bin

int main(void)
{
	unsigned char midstate_a[256+32];
	void *pmidstate = (void *) (((long) midstate_a+31L) & -32L);
	unsigned char hashout_a[256+32];
	unsigned char *phashout = (unsigned char *) (((long) hashout_a+31L) & -32L);
	unsigned char buf[256];
	FILE *outfile;
	int i;

	Blake2PrepareMidstate4(pmidstate, testdata);
	outfile = fopen("out.bin", "wb");

	for (i=0; i<1048576; i+=4) {
		Blake2Run4(phashout, pmidstate, i);
		memcpy(buf, phashout, 50);
		memcpy(buf+50, phashout+64, 50);
		memcpy(buf+100, phashout+128, 50);
		memcpy(buf+150, phashout+192, 50);
		fwrite(buf, 200, 1, outfile);	
	}

	fclose(outfile);

	return 0;
}
