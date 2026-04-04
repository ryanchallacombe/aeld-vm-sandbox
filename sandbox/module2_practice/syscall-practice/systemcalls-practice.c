#define _XOPEN_SOURCE
//#include "systemcalls.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <libgen.h>
#include <fcntl.h>


int main(int argc, char *argv[])
{	
	int test_no = 0;
	
	///////////////////////
	// Select test number
	//////////////////////
	test_no = 6;
	
	///////////////////////
	// declarations
	///////////////////////
	// char *cmd_path = "/usr/bin/nano";
	//char *const args[] = {"nano", "/home/ryan/projects/sandbox/module2_practice/textfile.txt", NULL}; 
	// char *cmd_path = "/usr/bin/test";
	// char *const args[] = {"test","-f","echo", NULL}; 
	// char *cmd_path = "/bin/sh";
	// char *const args[] = {"sh", "-c", "echo home is $HOME", NULL}; 
	char *cmd_path = "/bin/echo";
	char *const args[] = {"echo", "home is $HOME", NULL}; 
	//"/bin/sh", "-c", "echo home is $HOME"
	pid_t pid;
	int func_ret=99, status, ret;
	
	switch (test_no) {
	
		case 0:
			printf("This test number does nothing. Exiting...\n");
			break;
	
		case 1:		// fork() demonstration
			

			pid = fork();
			
			if (pid > 0)
				printf("This message is sent from the parent of pid %d\n", pid);
			else if (pid == 0)
				printf("This message is from the child process\n");
			else if (pid == -1)
				perror("fork");
			break;
		
		case 2:		// execl demonstration
			ret = execl("/bin/vi", "vi", NULL);
			if (ret == -1)
				perror("execl");
			break;
			
		case 3: 	// execv demonstration
			
			// call execv to open the following file in the nano editor
			// /home/ryan/projects/sandbox/module2_practice/textfile.txt
			    
			func_ret = execv("/usr/bin/nano", args);
			
			if (func_ret == -1)
				perror("execv");

			break;
			
		case 4:		// fork + execv + wait
			
			pid = fork();   

			if (pid == -1)
				perror("fork");
			else if (pid == 0) {	// this is executed in the child process
				/* How to use execv:
				// int execv(const char *path, char *const argv[]);
				// the path needs to be the full path including the command to be executed
				// the first item in the vector argv[] needs to be the command itself without the path
				// the remaining items are arguments to be passed into the command
				*/
				
				func_ret = execv(cmd_path, args);
				
				// None of the below in this 'else if' should run if execv runs correctly
				printf("** Execv returned:%d\n", func_ret);
				if (func_ret == -1)
					perror("execv");
				printf("This should not run\n");
			}
			
			//printf("** Execv returned:%d\n", func_ret);
			if (waitpid(pid, &status, 0) == -1) 
				return -1;
			else if (WIFEXITED(status)) {
				printf("Command returned status: %d\n", WEXITSTATUS(status));
				return WEXITSTATUS(status);
			}
			
			break;
			
		case 5: 	// test dirname and basename functions from libgen.h
			printf("full path is: %s\n", cmd_path);
			char *base;
			base = basename(cmd_path);
			printf("Base name=%s\n", base);

			break;
			
		case 6:		// writing execv output to a file
		
			// open the file passed into the function
			const char *outputfile = "dummy_output.txt";
			int fd = open(outputfile, O_WRONLY|O_TRUNC|O_CREAT, 0666);
			
			pid = fork();   

			if (pid == -1)
				perror("fork");
			else if (pid == 0) {	// this is executed in the child process
				/* How to use execv:
				// int execv(const char *path, char *const argv[]);
				// the path needs to be the full path including the command to be executed
				// the first item in the vector argv[] needs to be the command itself without the path
				// the remaining items are arguments to be passed into the command
				*/
				
				// this sets fd 1 (STDOUT) to the same file descriptor as fd which is the file name passed into the function
				// so from henceforth, if this code doesnt' have an error, anything going to STDOUT goes to the file name
				// This actually opens the file with fd = 1
				
				if (dup2(fd, 1) < 0) 
					perror("dup2"); 
				
				func_ret = execv(cmd_path, args);
				
				// None of the below in this 'else if' should run if execv runs correctly
				printf("** Execv returned:%d\n", func_ret);
				if (func_ret == -1)
					perror("execv");
				printf("This should not run\n");
			}
			
			close(fd);
			
			//printf("** Execv returned:%d\n", func_ret);
			if (waitpid(pid, &status, 0) == -1) 
				return -1;
			else if (WIFEXITED(status)) {
				printf("Command returned status: %d\n", WEXITSTATUS(status));
				return WEXITSTATUS(status);
			}
			
			//close(1);
			
			break;
			
			
		default:
			break;
	}
	

	return 0;
}

