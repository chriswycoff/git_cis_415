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
#include "command.h"
/*---------------------------------------------------------------------------*/


int exit_function(char * line_buffer){
	free(line_buffer);
	printf("\n");
	exit(0);
}

int file_io_mode(int argc, char *argv[]) {

	if (argc != 3){
		printf("USAGE: ./lab_2_file_io -f <filename> \n");
		exit(1);
	}

	char* file_string = "-f";

	if (strcmp(file_string, argv[1])){
			exit(1);
		}


	setbuf(stdout, NULL);
	
	char *line_buffer;
	size_t bufsize = 10000;
	//size_t characters;

	//char *exit_char = "exit";
	// file stuff
	FILE *fp ;
	FILE *new_file;

	fp = fopen(argv[2], "r");


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

/*-----------------------------Program Main----------------------------------*/
int main(int argc, char *argv[]) {

	char *dirName = "hello";

	char *sourcePath = "src";

	char *destinationPath = "dst";

	char *filename = "file";

	listDir(); /*for the ls command*/

	showCurrentDir(); /*for the pwd command*/

	makeDir(dirName); /*for the mkdir command*/

	changeDir(dirName); /*for the cd command*/

	copyFile(sourcePath, destinationPath); /*for the cp command*/

	moveFile(sourcePath, destinationPath); /*for the mv command*/

	deleteFile(filename); /*for the rm command*/

	displayFile(filename); /*for the cat command*/


	if (argc == 3){
		file_io_mode(argc, argv);
	}

	if (argc > 3 || argc == 2){
		printf("USAGE: ./lab_2_file_io -f <filename> \n");
		exit(1);
	}

	setbuf(stdout, NULL);
	
	char *line_buffer;
	size_t bufsize = 2048;
	size_t characters;

	char *exit_char = "exit";


	/* Main Function Variables */
	line_buffer = (char *)malloc(bufsize * sizeof(char));

	char * original_line = line_buffer;
	
	if (line_buffer == NULL){
		printf("ERROR in allocating buffer\n");
		exit(1);
	}
	/* Allocate memory for the input buffer. */
	

	/*main run loop*/
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


		int token_counter = 0;

		if (token != NULL){
			printf("\n");
		}

		while(token != NULL){
			if (*token == ' '){
			
			token = strtok_r(NULL, " ",&line_buffer);
			}

			
			printf("T%d: %s\n", token_counter, token);
			token = strtok_r(NULL, " ",&line_buffer);
			
			token_counter += 1; 
		}


	}
	
	/*Free the allocated memory*/

	return 1;
}
/*-----------------------------Program End-----------------------------------*/
