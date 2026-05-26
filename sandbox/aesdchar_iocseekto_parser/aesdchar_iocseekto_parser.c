/**
 *
 * Add special handling for socket write commands to your aesdsocket server application 
 * 		When the string sent over the socket equals AESDCHAR_IOCSEEKTO:X,Y where X and Y are unsigned decimal integer values, the X should be considered the write command to seek into and the Y should be considered the offset within the write command. 
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define IOC_PFX		AESDCHAR_IOCSEEKTO

int main(void)
{

	// Setup
	const char* ioc_pfx ="AESDCHAR_IOCSEEKTO";
	const char* seek1 = "AESDCHAR_IOCSEEKTO:6,7";
	printf("seek1: %s\n", seek1);
	printf("seek1[0]: %c\n", seek1[0]);

	// parse out prefix of the string
	size_t num_chars = 18;
	char letters_seek1[num_chars+1];
	strncpy(letters_seek1, seek1, num_chars);
	letters_seek1[num_chars] = '\0';			// null termination so that we can print it
	printf("letters_seek1: %s\n", letters_seek1);


	// compare the strings
	int cmp_ret = strncmp(ioc_pfx, letters_seek1, num_chars);
	printf("strncmp returned %i\n", cmp_ret);

	// parse out numbers
	// X is in position 19
	// Y is in position 21
	int x_pos = 19, y_pos = 21;
	char x = seek1[x_pos];
	char y = seek1[y_pos];
	printf("x = %c, y = %c\n", x, y);
	int x_num = x - '0';
	int y_num = y - '0';
	printf("x = %i, y = %i\n", x_num, y_num);
	
	return 0;
}