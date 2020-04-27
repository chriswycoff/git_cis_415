// Part 1  MCP v1.0


// do we need -f flag? 
// do we need output.txt???

#include "header.h"

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


int exit_function(char * line_buffer, char ***the_args, char** the_programs){
	free(line_buffer);
	printf("getting here no prob\n");
	// freeing the arguments
	for (int i = 0; i < 2048; i++){
		for (int j = 0; j < 2048; j++){
			free(the_args[i][j]);
		}
		printf("getting here no prob\n");
		free(the_args[i]);
	}

	free(the_args);


	// freeing the programs

	for (int i = 0; i < 2048; i++){
		free(the_programs[i]);
	}
	free(the_programs);

	printf("\n");
	exit(0);
}



int main(int argc, char *argv[]) {

	printf("RUNNING PART 1\n");
	//dummy_fucntion();

	//BEGIN parsing input file
///////////////////////////////////////////////////////////////////////////////////////////////////
	char *line_buffer;
	char*** the_args;
	char** the_programs;


	size_t bufsize = 10000;
	FILE *fp ;

	fp = fopen(argv[1], "r");

	if (fp == NULL){
		printf("invalid file name: %s\n",argv[2]);
		exit(1);
	}

	// allocations
	// allocate for lines
	line_buffer = (char *)malloc(bufsize * sizeof(char));

	// allocate for arguments
	the_args = (char ***)malloc(2048 * sizeof(char **));

	for (int i = 0; i < 2048; i++){
		the_args[i] = (char **)malloc(2048 * sizeof(char *));
		for (int j = 0; j < 2048; j++){
			the_args[i][j] = (char *)malloc(2048 * sizeof(char));
		}
	}

	// allocate for programs

	the_programs = (char **)malloc(2048 * sizeof(char *));

	for (int i = 0; i < 2048; i++){
		the_args[i] = (char **)malloc(2048 * sizeof(char));
	}


	// end allocations
	int current_program = 0;


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

		
		for (int i = 0; i< token_counter; i++){
			printf("%s\n",tokens[i]);

		}
		// printed tokens for debugging


		// add a NULL to tokens
		token_counter += 1; 
		tokens[token_counter]= NULL;


		the_args[current_program] = tokens;

		//printf("THIS: %s\n",the_args[current_program][0]);

		the_programs[current_program] = the_args[current_program][0];

		current_program+=1;

	}

	//END parsing input file

///////////////////////////////////////////////////////////////////////////////////////////////////

	int number_of_programs = current_program;
	printf("Num Programs: %d\n",number_of_programs);
	int program_iterator = 0;
	int argument_iterator = 0;
	char* argument = the_args[0][argument_iterator];


	for (int i = 0; i < number_of_programs; i++ ){
		printf("%d\n",i );
		while(argument != NULL){
			printf(" hi: %s\n", argument);
			argument_iterator += 1;
			argument = the_args[program_iterator][argument_iterator];
		}
		program_iterator+=1;
		argument_iterator = 0;
		argument = the_args[program_iterator][argument_iterator];
	}


	/// exiting

	exit_function(line_buffer,the_args,the_programs);


	return 1;

}