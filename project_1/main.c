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
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "command.h"
/*---------------------------------------------------------------------------*/


int exit_function(char * line_buffer){
	free(line_buffer);
	printf("\n");
	exit(0);
}

int file_io_mode(int argc, char *argv[]) {

	int fd_1 = open("output.txt", O_CREAT|O_RDWR, 0777);

	dup2(fd_1, 1); 

	//showCurrentDir(); 
	//listDir(); 
	makeDir("my_dir");
	exit(1);

	if (argc != 3){
		printf("USAGE: ./lab_2_file_io -f <filename> \n");
		exit(1);
	}

	char* file_string = "-f";

	if (strcmp(file_string, argv[1])){
			printf("USAGE: ./lab_2_file_io -f <filename> \n");
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


	//listDir(); //for the ls command

	//makeDir("my_dir_1");  //for the mkdir command

	//showCurrentDir(); //for the pwd command

	//showCurrentDir();

	//changeDir("../.."); //for the cd command

	//showCurrentDir();

	//copyFile("test.txt","test_dest.txt"); //for the cp command

	//moveFile("test.txt", "../test.txt"); //for the mv command

	//deleteFile("test.txt"); //for the rm command

	//displayFile("input.txt"); //for the cat command

	exit(1);

	file_io_mode(argc, argv);

	if (argc == 3){
		file_io_mode(argc, argv);
	}

	if (argc > 3 || argc == 2){
		printf("USAGE: ./pseudo-shell -f <filename> \nUSAGE: ./pseudo-shell\n");
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
