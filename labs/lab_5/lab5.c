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


void process_tester(){
	while(1){
		printf("process: %d going\n",getpid());
		sleep(2);
	}
}

void signaler_function(pid_t* the_ids, int the_signal){
	for (int i = 0; i < 5; i++ ){
		sleep(1);
		printf("Parent process <%d> - Sending Signal: <%d>\n",getpid(), the_signal);
		kill(the_ids[i],the_signal);
		
	}
	// now terminate everything 
	for (int i = 0; i < 5; i++ ){
		sleep(1);
		printf("Parent process <%d> - Sending Signal: <%d>\n",getpid(), the_signal);
		kill(the_ids[i],SIGINT);
	}

}

int main(int argc, char *argv[]) {


//////////////////////////////////////////////////////////////

	char* program = "./iobound";

	char* arguments[2] = {"./iobound", NULL};

	int num_processes = 5; 

	pid_t the_ids[num_processes];

	int signumber; 
	sigset_t sigset;
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGUSR1);
	sigprocmask(SIG_BLOCK, &sigset, NULL);


	for (int process_iterator = 0; process_iterator< num_processes; process_iterator++ ){

		the_ids[process_iterator] = fork();

		if (the_ids[process_iterator] == 0){


			printf("Child Process <%d> wating for SIGUSR1... \n", getpid());
			if (sigwait(&sigset, &signumber) == 0){


				printf("Child Process <%d> Recieved signal: SIGUSR1... \n", getpid());
				// executing here
				if ( execvp(program, &arguments[2] ) < 0){
					printf("PROBLEM STARTIG PROCESS\n");

				}


			}

			else{
				printf("Error waiting the process: %d \n", getpid());
			}

		}

	}
	// parent to call signaler function here

	signaler_function(the_ids, SIGUSR1);

	

}









