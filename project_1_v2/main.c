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

/*-----------------define command keys----------------------------------------------------*/

#define A_NULL 0
#define SEMICOLON 1
#define LS 2
#define PWD 3
#define RM 4
#define CAT 5
#define MKDIR 6
#define CD 7
#define CP 8
#define MV 9

#define INVALID -1




/*---------------------------------------------------------------------------*/

typedef struct{
	char *key;
	int value;
}legal_commands;


legal_commands command_lookup[] = {
	{";", SEMICOLON}, { "ls", LS }, { "pwd", PWD }, { "rm", RM },
	{ "cat", CAT }, { "mkdir", MKDIR }, { "cd", CD }, 
	{ "cp", CP }, { "mv", MV }
};

int number_of_keys = 9;

int get_value_from_string_key(char *a_key)
{
	if (a_key == NULL){
		return A_NULL;
	}
    for (int i=0; i < number_of_keys; i++) {
        legal_commands *sym = &command_lookup[i];
        if (strcmp(sym->key, a_key) == 0){
            return sym->value;
        }
    }
    // if gets to here there was no return thus return -1
   
    return INVALID;
}


int exit_function(char * line_buffer){
	free(line_buffer);
	printf("\n");
	exit(0);
}

int file_io_mode(int argc, char *argv[]) {


	int fd_1 = open("output.txt", O_CREAT|O_RDWR, 0777);

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


	FILE *fp ;


	fp = fopen(argv[2], "r");

	if (fp == NULL){
		printf("invalid file name: %s\n",argv[2]);
		exit(1);
	}

	dup2(fd_1, 1); 


	//new_file = fopen("output.txt", "w");

	// end file stuff

	/* Main Function Variables */
	line_buffer = (char *)malloc(bufsize * sizeof(char));


	char * original_line = line_buffer;
	
	if (line_buffer == NULL){
		printf("ERROR in allocating buffer\n");
		exit(1);
	}
	int characters = 0;

	while((characters = getline(&line_buffer, &bufsize, fp)) >= 0) {
		//printf("gettin here\n");
		if (line_buffer[characters-1] == '\n'){
			line_buffer[characters-1] = '\0';
			characters -= 1;
		}



		char* tokens[2048];

		for(int i = 0; i<2048; i++){
			tokens[i] = "invalid";
		}

		char *token = strtok_r(line_buffer, " ", &line_buffer);

		int token_counter = 0;

		tokens[token_counter]= token;


		if (token != NULL){
			printf("\n");
		}

		//gather tokens below
		while(token != NULL){
			//printf("T%d: %s\n", token_counter, token);
			token = strtok_r(NULL, " ",&line_buffer);
			token_counter += 1; 
			tokens[token_counter]= token;
		}

		// gathered tokens analysed below
		//
		for(int i = 0; i < token_counter; ++i){
			// other attemp without switch below
			//printf("token%d: %s\n", i, tokens[i]);
			//if tokens[i]
			/*
			if (!strcmp(tokens[i],lfcat_string)){
				lfcat();
			}
			else if (!strcmp(tokens[i],lfcat_string))
			*/
			// end other attempt

			switch(get_value_from_string_key(tokens[i])) {
				case LS:
					if (get_value_from_string_key(tokens[i+1]) == -1){
						printf("Error! unsupported parameter!\n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+1]) > 1){
						printf(" Error! Incorrect syntax. No control code found \n");
						exit_function(original_line);
					}
					listDir();
					break; ///////////////////////////////////////////////////

				case SEMICOLON:
					if (tokens[i+1] == NULL){
						printf("Error unrecognized command!\n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+1]) == -1){
						printf("Error unrecognized command!\n");
						exit_function(original_line);
					}
					break; ///////////////////////////////////////////////////

				case PWD:
					if (get_value_from_string_key(tokens[i+1]) == -1 || 
						get_value_from_string_key(tokens[i+1]) > 1){
						printf("Error! unsupported parameter!\n");
						exit_function(original_line);
					}
					showCurrentDir();
					break; ///////////////////////////////////////////////////

				case MKDIR:
					if ((get_value_from_string_key(tokens[i+1]) == 0) || 
					(get_value_from_string_key(tokens[i+1]) == 1)){
						printf("Error! No parameter given!\n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+1]) > 1) {
						printf("Error! unsupported parameter!\n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+2]) > 1) {
						printf(" Error! Incorrect syntax. No control code found \n");
						exit_function(original_line);
					}

					makeDir(tokens[i+1]);
					i += 1; // increment i an extra increment
					break; ///////////////////////////////////////////////////

				case RM:
					if ((get_value_from_string_key(tokens[i+1]) == 0) || 
					(get_value_from_string_key(tokens[i+1]) == 1)){
						printf("Error! No parameter given!\n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+1]) > 1) {
						printf("Error! unsupported parameter!\n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+2]) > 1) {
						printf(" Error! Incorrect syntax. No control code found \n");
						exit_function(original_line);
					}

					deleteFile(tokens[i+1]);
					i += 1; // increment i an extra increment
					break; ///////////////////////////////////////////////////

				case CAT:
					if ((get_value_from_string_key(tokens[i+1]) == 0) || 
					(get_value_from_string_key(tokens[i+1]) == 1)){
						printf("Error! No parameter given!\n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+1]) > 1) {
						printf("Error! unsupported parameter!\n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+2]) > 1) {
						printf(" Error! Incorrect syntax. No control code found \n");
						exit_function(original_line);
					}

					displayFile(tokens[i+1]);
					i += 1; // increment i an extra increment
					break; ///////////////////////////////////////////////////

				case CD:
					if ((get_value_from_string_key(tokens[i+1]) == 0) || 
					(get_value_from_string_key(tokens[i+1]) == 1)){
						printf("Error! No parameter given!\n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+1]) > 1) {
						printf(" Error! Incorrect syntax. No control code found \n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+2]) > 1) {
						printf(" Error! Incorrect syntax. No control code found \n");
						exit_function(original_line);
					}

					changeDir(tokens[i+1]);
					i += 1; // increment i an extra increment
					break; ///////////////////////////////////////////////////

				case CP:
					if ((get_value_from_string_key(tokens[i+1]) == 0) || 
					(get_value_from_string_key(tokens[i+1]) == 1)){
						printf("Error! No 1st parameter given!\n");
						exit_function(original_line);
					}
					if ((get_value_from_string_key(tokens[i+2]) == 0) || 
					(get_value_from_string_key(tokens[i+2]) == 1)){
						printf("Error! No 2nd parameter given!\n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+1]) > 1) {
						printf(" Error! Incorrect syntax. No control code found \n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+2]) > 1) {
						printf(" Error! Incorrect syntax. No control code found \n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+3]) > 1) {
						printf(" Error! Incorrect syntax. No control code found \n");
						exit_function(original_line);
					}

					copyFile(tokens[i+1],tokens[i+2]);
					i += 2; // increment i an extra increment
					break; ///////////////////////////////////////////////////

				case MV:
					if ((get_value_from_string_key(tokens[i+1]) == 0) || 
					(get_value_from_string_key(tokens[i+1]) == 1)){
						printf("Error! No 1st parameter given!\n");
						exit_function(original_line);
					}
					if ((get_value_from_string_key(tokens[i+2]) == 0) || 
					(get_value_from_string_key(tokens[i+2]) == 1)){
						printf("Error! No 2nd parameter given!\n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+1]) > 1) {
						printf(" Error! Incorrect syntax. No control code found \n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+2]) > 1) {
						printf(" Error! Incorrect syntax. No control code found \n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+3]) > 1) {
						printf(" Error! Incorrect syntax. No control code found \n");
						exit_function(original_line);
					}
				
					moveFile(tokens[i+1],tokens[i+2]);
					i += 2; // increment i an extra increment
					break; ///////////////////////////////////////////////////

				case A_NULL:
					break; ///////////////////////////////////////////////////

				default:
					printf("Error unrecognized command!!!!!!!\n");
					exit_function(original_line);

			}

		}


	

	}



	
	/*Free the allocated memory*/
	fclose (fp); 
	exit_function(original_line);

	return 1;
}

/*-----------------------------Program Main----------------------------------*/
int main(int argc, char *argv[]) {


	//  //// list of commands ///
	///char * lfcat_string = "lfcat";

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

		char* tokens[2048];

		for(int i = 0; i<2048; i++){
			tokens[i] = "invalid";
		}

		char *token = strtok_r(line_buffer, " ", &line_buffer);

		int token_counter = 0;

		tokens[token_counter]= token;

		if (token != NULL){
			printf("\n");
		}

		//gather tokens below
		while(token != NULL){
			//printf("T%d: %s\n", token_counter, token);
			token = strtok_r(NULL, " ",&line_buffer);
			token_counter += 1; 
			tokens[token_counter]= token;
		}

		// gathered tokens analysed below
		//
		for(int i = 0; i < token_counter; ++i){
			
			// other attemp without switch below
			//printf("token%d: %s\n", i, tokens[i]);
			//if tokens[i]
			/*
			if (!strcmp(tokens[i],lfcat_string)){
				lfcat();
			}
			else if (!strcmp(tokens[i],lfcat_string))
			*/
			// end other attempt
			//for (int j = 0 ; j< 10; j++){
				//printf("i: %d : %s\n",i, tokens[j]);
			//}


			//printf("%d\n", token_counter);

			switch(get_value_from_string_key(tokens[i])) {
				case LS:
					if (get_value_from_string_key(tokens[i+1]) == -1){
						printf("Error! unsupported parameter!\n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+1]) > 1){
						printf(" Error! Incorrect syntax. No control code found \n");
						exit_function(original_line);
					}
					listDir();
					break; ///////////////////////////////////////////////////

				case SEMICOLON:
					if (tokens[i+1] == NULL){
						printf("Error unrecognized command!\n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+1]) == -1){
						printf("Error unrecognized command!\n");
						exit_function(original_line);
					}
					break; ///////////////////////////////////////////////////

				case PWD:
					if (get_value_from_string_key(tokens[i+1]) == -1 || 
						get_value_from_string_key(tokens[i+1]) > 1){
						printf("Error! unsupported parameter!\n");
						exit_function(original_line);
					}
					showCurrentDir();
					break; ///////////////////////////////////////////////////

				case MKDIR:
					if ((get_value_from_string_key(tokens[i+1]) == 0) || 
					(get_value_from_string_key(tokens[i+1]) == 1)){
						printf("Error! No parameter given!\n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+1]) > 1) {
						printf("Error! unsupported parameter!\n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+2]) > 1) {
						printf(" Error! Incorrect syntax. No control code found \n");
						exit_function(original_line);
					}

					makeDir(tokens[i+1]);
					i += 1; // increment i an extra increment
					break; ///////////////////////////////////////////////////

				case RM:
					if ((get_value_from_string_key(tokens[i+1]) == 0) || 
					(get_value_from_string_key(tokens[i+1]) == 1)){
						printf("Error! No parameter given!\n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+1]) > 1) {
						printf("Error! unsupported parameter!\n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+2]) > 1) {
						printf(" Error! Incorrect syntax. No control code found \n");
						exit_function(original_line);
					}

					deleteFile(tokens[i+1]);
					i += 1; // increment i an extra increment
					break; ///////////////////////////////////////////////////

				case CAT:
					if ((get_value_from_string_key(tokens[i+1]) == 0) || 
					(get_value_from_string_key(tokens[i+1]) == 1)){
						printf("Error! No parameter given!\n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+1]) > 1) {
						printf("Error! unsupported parameter!\n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+2]) > 1) {
						printf(" Error! Incorrect syntax. No control code found \n");
						exit_function(original_line);
					}

					displayFile(tokens[i+1]);
					i += 1; // increment i an extra increment
					break; ///////////////////////////////////////////////////

				case CD:
					if ((get_value_from_string_key(tokens[i+1]) == 0) || 
					(get_value_from_string_key(tokens[i+1]) == 1)){
						printf("Error! No parameter given!\n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+1]) > 1) {
						printf(" Error! Incorrect syntax. No control code found \n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+2]) > 1) {
						printf(" Error! Incorrect syntax. No control code found \n");
						exit_function(original_line);
					}

					changeDir(tokens[i+1]);
					i += 1; // increment i an extra increment
					break; ///////////////////////////////////////////////////

				case CP:
					if ((get_value_from_string_key(tokens[i+1]) == 0) || 
					(get_value_from_string_key(tokens[i+1]) == 1)){
						printf("Error! No 1st parameter given!\n");
						exit_function(original_line);
					}
					if ((get_value_from_string_key(tokens[i+2]) == 0) || 
					(get_value_from_string_key(tokens[i+2]) == 1)){
						printf("Error! No 2nd parameter given!\n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+1]) > 1) {
						printf(" Error! Incorrect syntax. No control code found \n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+2]) > 1) {
						printf(" Error! Incorrect syntax. No control code found \n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+3]) > 1) {
						printf(" Error! Incorrect syntax. No control code found \n");
						exit_function(original_line);
					}

					copyFile(tokens[i+1],tokens[i+2]);
					i += 2; // increment i an extra increment
					break; ///////////////////////////////////////////////////

				case MV:
					if ((get_value_from_string_key(tokens[i+1]) == 0) || 
					(get_value_from_string_key(tokens[i+1]) == 1)){
						printf("Error! No 1st parameter given!\n");
						exit_function(original_line);
					}
					if ((get_value_from_string_key(tokens[i+2]) == 0) || 
					(get_value_from_string_key(tokens[i+2]) == 1)){
						printf("Error! No 2nd parameter given!\n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+1]) > 1) {
						printf(" Error! Incorrect syntax. No control code found \n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+2]) > 1) {
						printf(" Error! Incorrect syntax. No control code found \n");
						exit_function(original_line);
					}
					if (get_value_from_string_key(tokens[i+3]) > 1) {
						printf(" Error! Incorrect syntax. No control code found \n");
						exit_function(original_line);
					}
					
					moveFile(tokens[i+1],tokens[i+2]);
					i += 2; // increment i an extra increment
					break; ///////////////////////////////////////////////////

				case A_NULL:
					break; ///////////////////////////////////////////////////

				default:
					printf("Error unrecognized command!!!!!!!\n");
					exit_function(original_line);

			}

		}


	}
	
	/*Free the allocated memory*/

	return 1;
}
/*-----------------------------Program End-----------------------------------*/