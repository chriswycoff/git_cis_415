/*
* Description: <write a brief description of your lab>
*
* Author: Christopher Wycoff 951078512
*
* Date: <today's date>
*
* Notes: 
* 1. <add notes we should consider when grading>
somewhat referenced this code:
https://c-for-dummies.com/blog/?p=1112
*/

/*-------------------------Preprocessor Directives---------------------------*/
#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*---------------------------------------------------------------------------*/

/*-----------------------------Program Main----------------------------------*/
int main() {
	setbuf(stdout, NULL);
	
	char *line_buffer;
	size_t bufsize = 256;
	size_t characters;


	/* Main Function Variables */
	line_buffer = (char *)malloc(bufsize * sizeof(char));
	
	if (line_buffer == NULL){
		printf("ERROR in allocating buffer\n");
		exit(1);
	}
	/* Allocate memory for the input buffer. */
	
	/*main run loop*/
	while(1){
		printf(">>>\n");
		characters = getline(&line_buffer, &bufsize, stdin);
		/*
		if !(strcmp('exit', buffer)){
			exit(0)
		}
		*/
		printf(" you typed %zu characters \n", characters);

		printf("%s ,is what you typed \n", line_buffer);
		/* Print >>> then get the input string */

		/* Tokenize the input string */

		/* Display each token */
		
		/* If the user entered <exit> then exit the loop */
		exit(0);
	}
	
	/*Free the allocated memory*/

	return 1;
}
/*-----------------------------Program End-----------------------------------*/
