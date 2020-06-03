// header.h

#include <stdio.h>
#include <signal.h>


int dummy_fucntion(){
	printf("I am dumb\n");

	return 0;
}

int exit_function(int status, char * line_buffer, char ***the_args, char** the_programs, 
	int * arguments_per_program, int number_of_programs, char*** copy_of_args);

void signal_handler(int sig, siginfo_t * siginfo, void *context);

int display_status(long tgid);

void get_exec_time(long tgid);

void display_memory(long tgid);

void display_io_read(long tgid);

void display_io_write(long tgid);