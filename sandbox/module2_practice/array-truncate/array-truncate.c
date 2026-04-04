#define _XOPEN_SOURCE
//#include "systemcalls.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char *argv[])
{	
	
			int count = 3;
			
			char *command[] = {"make", "my", "day", NULL};
			for (int i = 0; i <= count; i++) {
				printf("command[%d]=%s\n", i, command[i]);
			}

			// copy command into a new array because we don't need command[0] in the second execv argument
			
			char *cmd[count];
			for (int i = 0; i < count; i++)
			{
				cmd[i] = command[i+1];
				printf("cmd[%d]=%s\n", i, cmd[i]);	
			}
			
			printf("command[0]=%s\n", command[0]);	
			

	return 0;
}

