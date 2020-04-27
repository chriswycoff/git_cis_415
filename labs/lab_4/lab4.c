// lab4.c

//Christopher Wycoff 951078512


// process id is 0 in the child
// avoid fork bomb

// proc

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){

	pid_t pid;

	printf("Main starting, m pid is %d\n\n", getpid());

	pid = fork();

	if (pid == 0){

		printf("This is the child process, my pid is %d, my parent pid is %d\n", getpid(), getppid());
		sleep(2);
	}
	else{
		//waitpid(); 
		wait(0);
		printf("Child finished, main exiting, my pid is %d \n", getpid());
	}


}