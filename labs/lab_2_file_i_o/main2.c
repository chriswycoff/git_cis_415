/*
* Description: <write a brief description of your lab>
*
* Author: Christopher Wycoff 951078512
*
* Date: <today's date>
*
* Notes: 
* 1. <add notes we should consider when grading>
somewhat referenced these codes:
https://c-for-dummies.com/blog/?p=1112
https://www.geeksforgeeks.org/strtok-strtok_r-functions-c-examples/
*/

/*-------------------------Preprocessor Directives---------------------------*/
#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*---------------------------------------------------------------------------*/


int exit_function(char * line_buffer){
	free(line_buffer);
	printf("\n");
	exit(0);
}

/*-----------------------------Program Main----------------------------------*/
int main(int argc, char *argv[]) {

	if (argc != 2){
		printf("USAGE: ./lab_2_file_io <filename> \n");
		exit(1);
	}
	setbuf(stdout, NULL);
	
	char *line_buffer;
	size_t bufsize = 2048;
	//size_t characters;

	//char *exit_char = "exit";
	// file stuff
	FILE *fp ;
	// end file stuff

	fp = fopen(argv[1], "r");
	/* Main Function Variables */
	line_buffer = (char *)malloc(bufsize * sizeof(char));

	char * original_line = line_buffer;
	
	if (line_buffer == NULL){
		printf("ERROR in allocating buffer\n");
		exit(1);
	}


	while(fgets(line_buffer, bufsize, fp) != NULL) {

		char* token = strtok_r(line_buffer, " ", &line_buffer);

		//char* next_token = "hello";

		int token_counter = 0;
		
		if (token != NULL){
			printf("\n");
		}
		
		while(token != NULL){
			
			if (*token == ' '){
			//printf("T%d%s\n", token_counter, token);
			token = strtok_r(NULL, " ",&line_buffer);
			}

			
			printf("T%d: %s\n", token_counter, token);
			token = strtok_r(NULL, " ",&line_buffer);
			//next_token = strtok_r(NULL, " ",&line_buffer);
			token_counter += 1; 
		}

	}

	/* Allocate memory for the input buffer. */
	

	/*main run loop*/
	/*
	while(1){
		printf(">>>");

		characters = getline(&line_buffer, &bufsize, stdin);

		if (line_buffer[characters-1] == '\n'){
			line_buffer[characters-1] = '\0';
			characters -= 1;
		}
		
		if (!strcmp(exit_char, line_buffer)){
			exit_function(original_line);
		}

		char* token = strtok_r(line_buffer, " ", &line_buffer);

		//char* next_token = "hello";

		int token_counter = 0;

		int flag = 0;
		if (token != NULL){
			printf("\n");
		}
		*/

		//printf(" you typed %zu characters \n", characters);

		//printf("%s ,is what you typed \n", line_buffer);
		/* Print >>> then get the input string */

		/* Tokenize the input string */

		/* Display each token */
		
		/* If the user entered <exit> then exit the loop */
		//exit(0);
	//}
	
	/*Free the allocated memory*/
	fclose (fp); 
	exit_function(original_line);

	return 1;
}
/*-----------------------------Program End-----------------------------------*/
