// Project 3 "quacker"
// Christopher Wycoff

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <unistd.h>

//////// START defines  ////////////////////////////////////////////////

#define URLSIZE 100
#define CAPSIZE 100
#define MAXENTRIES 10
#define MAXTOPICS 10
#define NUMPROXIES 10

//////// END defines  ////////////////////////////////////////////////


//////// START global variables ////////////////////////////////////////////////


struct topicEntry{
  int entryNum;
  struct timeval timestamp;
  int pubId;
  char photoURL[URLSIZE];
  char photoCaption[CAPSIZE];
};

struct topic_queue{
	int	count, head, tail;
	struct topicEntry *entries;
};

struct topic_queue topic_queues[MAXTOPICS];
pthread_t pubs[NUMPROXIES];		// thread ID for publishers
pthread_t subs[NUMPROXIES];		// thread ID for subscribers
pthread_attr_t attr; 
pthread_mutex_t mutex[MAXTOPICS];

struct threadargs {
  int	id;
};

struct threadargs pubargs[NUMPROXIES];
struct threadargs subargs[NUMPROXIES];

void *publisher(void *param);		// publisher routine
void *subscriber(void *param);		// subscriber routine



//////// END global variables ////////////////////////////////////////////////

//////// Begin initialize ////////////////////////////////////////////////
void initialize() {
int i; //, j, k;

  // create the buffers
  for (i=0; i<MAXTOPICS; i++) {
    topic_queues[i].count = 0;	// # entries in buffer now
    topic_queues[i].head = 0;	// head index
    topic_queues[i].tail = 0;	// tail index
    topic_queues[i].entries = (struct topicEntry *) malloc(sizeof(struct topicEntry) * MAXENTRIES);
  }

  // create the buffer semaphores
  for (i=0; i<MAXTOPICS; i++) {
    pthread_mutex_init(&(mutex[i]), NULL);
    //    sem_init(&full[i], 0, 0);
    //    sem_init(&empty[i], 0, BUFFERSIZE);
  }

  pthread_attr_init(&attr);

} // initialize()

void exit_function(){
	for (int i=0; i<MAXTOPICS; i++) {
		free(topic_queues[i].entries);
	}
	exit(0);

}

//////// END initialize  ////////////////////////////////////////////////



////// BEGIN ENQUEUE /////////////////////////////////////////////////////
int enqueue(){
	//gettimeofday();
	printf("Calling enqueue\n");
	return 1;

}
////// END ENQUEUE /////////////////////////////////////////////////////


////// BEGIN MAIN /////////////////////////////////////////////////////
int main(int argc, char *argv[]){

	if (argc < 2){
		printf("USAGE ./server <int>\n");
		exit(-1);
	}

	int first_arg = atoi(argv[1]);

	printf("HELLO WORLD My int is %d\n" ,first_arg);
	initialize();
	exit_function();
}
////// END MAIN /////////////////////////////////////////////////////





