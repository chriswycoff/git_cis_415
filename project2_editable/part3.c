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
#include <sys/wait.h>
#include <signal.h>


int exit_function(int status, char * line_buffer, char ***the_args, char** the_programs, 
	int * arguments_per_program, int number_of_programs, char*** copy_of_args){

	
	//printf("getting here no prob\n");
	// freeing the arguments 
	for (int i = 0; i < number_of_programs; i++){
		for(int j = 0; j < (arguments_per_program[i]); j++){
			free(the_args[i][j]);
		}
		free(the_args[i]);
	}
	free(the_args);

	for (int i = 0; i < number_of_programs; i++){
		for(int j = 0; j < (arguments_per_program[i]+1); j++){
			free(copy_of_args[i][j]);
		}
		free(copy_of_args[i]);
	}

	free(copy_of_args);

	// freeing the programs
	for (int i = 0; i < number_of_programs; i++){
		free(the_programs[i]);
	}
	free(the_programs);

	// free line buffer
	free(line_buffer);

	//printf("\n");
	exit(status);
}




void handler_function_1(int sig){

	write(STDOUT_FILENO, "SIGNAL RECIEVED\n",16);

	/*
	sigset_t sigset;
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGUSR1);
	sigprocmask(SIG_BLOCK, &sigset, NULL);


	/// this will unblock if blocked
	if (sigwait(&sigset, &sig) == 0){
		printf("Unblocking here\n");
	}
	*/
	

}




int main(int argc, char *argv[]) {

	printf("RUNNING PART 1\n");
	//dummy_fucntion();

	//BEGIN parsing input file
///////////////////////////////////////////////////////////////////////////////////////////////////
	char *line_buffer;
	char*** the_args;
	char** the_programs;

	int arguments_per_program[256];

	for(int i = 0; i < 256; i++){
		arguments_per_program[i] = -1;
	}


	size_t bufsize = 1000;
	FILE *fp ;

	fp = fopen(argv[1], "r");

	if (fp == NULL){
		printf("invalid file name: %s\n",argv[2]);
		exit(1);
	}


	// allocations
	// allocate for lines

	// potentially read the number of arguments
	line_buffer = (char *)malloc(bufsize * sizeof(char));
	the_args = (char ***)malloc(256 * sizeof(char**));
	the_programs = (char **)malloc(256 * sizeof(char*));


	/*
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
	*/

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


		// malloc inner array of strings for tokens   
		the_args[current_program] = (char **)malloc(token_counter * sizeof(char*));

		// malloc strings for arguments
		for (int i = 0; i< token_counter; i++){
			the_args[current_program][i] = (char *)malloc(256 * sizeof(char));
			strcpy(the_args[current_program][i],tokens[i]);
		}
		// alloc for NULL terminator for exec
		//the_args[current_program][token_counter] = (char *)malloc(256 * sizeof(char));

		//the_args[current_program][token_counter] = NULL;

		the_programs[current_program] = (char *)malloc(256 * sizeof(char));

		strcpy(the_programs[current_program], tokens[0]);


		arguments_per_program[current_program] = token_counter;

		current_program+=1;
		

	}

	//END parsing input file

///////////////////////////////////////////////////////////////////////////////////////////////////

	
	int number_of_programs = current_program;
	

	/*
	printf("Num Programs: %d\n",number_of_programs);
	

	for(int i = 0; i < number_of_programs; i++ ){
		for(int j = 0; j < arguments_per_program[i]; j++){
			printf("ARGs for PRG%d:  %s\n", i, the_args[i][j]);
		}
	}

	printf("\n");

	for(int i = 0; i < number_of_programs; i++ ){
	printf("Program %d %s\n", i, the_programs[i]);
	}
	*/

	// malloc a copy of arguments ////////////////////////////////////////////

	int  num_args;

	// num_args = arguments_per_program[0];

	//char **copy_of_args;
	char ***copy_of_args;

	///// the_args = (char ***)malloc(256 * sizeof(char**)); assign new args

	copy_of_args = (char ***)malloc( (number_of_programs) * sizeof(char**));

	for (int program_number = 0; program_number < number_of_programs; program_number++){

		num_args = arguments_per_program[program_number];

		copy_of_args[program_number] = (char **)malloc( (num_args+1) * sizeof(char*));

		for (int i = 0; i < num_args; i++){
			copy_of_args[program_number][i] = (char *)malloc( 256 * sizeof(char));
			strcpy(copy_of_args[program_number][i], the_args[program_number][i]);
		}

		copy_of_args[program_number][num_args] = NULL;

	}

	/*
	copy_of_args = (char **)malloc( (num_args) * sizeof(char*));


	for (int i = 0; i < num_args; i++){
		copy_of_args[i] = (char *)malloc( 256 * sizeof(char));
		strcpy(copy_of_args[i], the_args[0][i]);
	}

	copy_of_args[num_args+1] = NULL;
	*/


	// end malloc ////////////////////////////////////////////

/////// BEGIN FORKING //////////////////////////////////////////////////////////////////////

	pid_t pid;

	printf("**********************************************************************\n");
	printf("MAIN LOGIC starting, m pid is %d\n\n", getpid());
	printf("**********************************************************************\n");
	printf("\n");

	//////////// create process array /////////////////
	pid_t the_ids[256];
	//////////// end create process array /////////////


	/*
	From Grayson Guan to Everyone: (01:39 PM)
 https://www.linuxprogrammingblog.com/code-examples/sigaction 
From Grayson Guan to Everyone: (01:53 PM)
 https://stackoverflow.com/questions/6326290/about-the-ambiguous-description-of-sigwait in main declear sigaction struct set function pointer inside struct to be your sighandler inside handler function, set block action, set sigwait
	*/

	//////////// create sig struct and handler /////////////

	struct sigaction signal_action_struct;
	// memset (&signal_action_struct, '\0', sizeof(signal_action_struct));
	signal_action_struct.sa_handler = handler_function_1;

	sigaction(SIGUSR1,&signal_action_struct,NULL);

	int signumber; 
	sigset_t sigset;
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGUSR1);
	sigprocmask(SIG_BLOCK, &sigset, NULL);

	/// this will unblock if blocked
	//if (sigwait(&sigset, &signumber) == 0){
	//	printf("Unblocking here\n");
	//}

	

	//////////// end create sig struct /////////////
	

	for (int fork_iterator = 0; fork_iterator < number_of_programs; fork_iterator++ ){

		the_ids[fork_iterator] = fork();

		// stop child //
		

		if (the_ids[fork_iterator] == 0){
			printf("just forked... current processs: %d \n", getpid());

			printf("sigwaiting the child: %d \n", getpid());
			//if (the_ids[fork_iterator] == 0){
				sigwait(&sigset, &signumber);


			//}


			printf("This is the child process, my pid is %d, my parent pid is %d\n", getpid(), getppid());
			// printf("My status is  %d\n\n",the_ids[fork_iterator] );
			printf("//////////////////////////////////////////////////////////\n");

			printf("Attempting to run: %s\n", the_programs[fork_iterator]);

			for (int i = 0; i <5; i++){
				printf("process %d in the rear with the gear \n", getpid() );
				sleep(2);
			}


			if ( execvp(the_programs[fork_iterator], copy_of_args[fork_iterator]) < 0){
				perror("execvp");
				//exit(-1);
					fclose (fp); 
					exit_function(-1, original_line, the_args, the_programs, arguments_per_program, number_of_programs,
					copy_of_args);
			}
			

				fclose (fp); 
				exit_function(0, original_line, the_args, the_programs, arguments_per_program, number_of_programs,
				copy_of_args);
			

		}

	}


	//sleep(2);
	/// for loop to bring children back to life
	for (int fork_iterator = 0; fork_iterator < number_of_programs; fork_iterator++ ){
		sleep(1);
		printf("waking up child back up %d\n", the_ids[fork_iterator]);
		kill(the_ids[fork_iterator],SIGUSR1);
		sleep(3);
	}

	/// for loop to 
	for (int fork_iterator = 0; fork_iterator < number_of_programs; fork_iterator++ ){
		sleep(1);
		printf("stopping child %d\n", the_ids[fork_iterator]);
		kill(the_ids[fork_iterator],SIGSTOP);
		sleep(3);
	}

		for (int fork_iterator = 0; fork_iterator < number_of_programs; fork_iterator++ ){
		sleep(1);
		printf("continuing child back up %d\n", the_ids[fork_iterator]);
		kill(the_ids[fork_iterator],SIGCONT);
		sleep(3);
	}

	// for loop to wait for the children
	for (int fork_iterator = 0; fork_iterator < number_of_programs; fork_iterator++ ){

		if (the_ids[fork_iterator] == 0){
			printf("BIG ISSUE CHILD DID NOT EXIT\n");
			exit_function(-1, original_line, the_args, the_programs, arguments_per_program, number_of_programs,
			copy_of_args);
		}
		
		else{
			//waitpid();
			printf("Waiting for child... \n");
			wait(0);//waitpid(the_ids[fork_iterator], NULL, __WALL);
			printf("Child finished, control back to parent: my pid is %d \n\n", getpid());
		}

	}
	
	/// exiting
	printf("All Processing Finished: parent exiting: my pid is %d \n", getpid());

	printf("//////////////////////////////////////////////////////////\n");
	
////////////////////////////////////////////////////////////////////////////////////////////
	


	fclose (fp); 
	exit_function(0, original_line, the_args, the_programs, arguments_per_program, number_of_programs,
		copy_of_args);

	//free(line_buffer);
	//exit(0);
	return 1;

}














