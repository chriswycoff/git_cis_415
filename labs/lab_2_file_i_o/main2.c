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
	FILE *new_file;

	fp = fopen(argv[1], "r");

	new_file = fopen("output.txt", "w");

	// end file stuff

	/* Main Function Variables */
	line_buffer = (char *)malloc(bufsize * sizeof(char));

	char * original_line = line_buffer;
	
	if (line_buffer == NULL){
		printf("ERROR in allocating buffer\n");
		exit(1);
	}


	while(getline(&line_buffer, &bufsize, fp) >= 0) {

		char* token = strtok_r(line_buffer, " ", &line_buffer);

		//char* next_token = "hello";

		int token_counter = 0;
		
		while(token != NULL){
			
			if (*token == ' '){
			//printf("T%d%s\n", token_counter, token);
			token = strtok_r(NULL, " ",&line_buffer);
			}

			
			fprintf(new_file,"T%d: %s\n", token_counter, token);
			token = strtok_r(NULL, " ",&line_buffer);
			//next_token = strtok_r(NULL, " ",&line_buffer);
			token_counter += 1; 
		}

	}

	
	/*Free the allocated memory*/
	fclose (fp); 
	exit_function(original_line);

	return 1;
}
/*-----------------------------Program End-----------------------------------*/
