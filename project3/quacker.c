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
#define MAXENTRIES 4
#define MAXTOPICS 2
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
	int entry_number;
	int	count, head, tail;
	struct topicEntry *entries;
};

struct topic_queue topic_queues[MAXTOPICS]; // data structure holding the topic queues

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

char time_print_buffer[30]; // time print buffer







//////// END global variables ////////////////////////////////////////////////

//////// Begin initialize ////////////////////////////////////////////////
void initialize() {
int i; //, j, k;

  // create the buffers
  for (i=0; i<MAXTOPICS; i++) {
  	topic_queues[i].entry_number = 1; // monotomically increasing number per topic
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
int enqueue(struct topic_queue * a_topic_queue){

	printf("Calling enqueue\n");

	if (a_topic_queue->count >= MAXENTRIES){
		printf("full queue \n");
		return -1;
	}
	// if this ^^^ does not happen there is room to enqueue
	a_topic_queue->entries[a_topic_queue->head].entryNum = a_topic_queue->entry_number;

	gettimeofday(&a_topic_queue->entries[a_topic_queue->head].timestamp, NULL);

	//increment stuff

	a_topic_queue->count++;

	a_topic_queue->entry_number++;

	a_topic_queue->head = (a_topic_queue->head + 1) % MAXENTRIES ;

	sleep(1);

	return 1;

}

////// END ENQUEUE /////////////////////////////////////////////////////


////// BEGIN MAIN /////////////////////////////////////////////////////
int main(int argc, char *argv[]){

	time_t curtime; // for printing purposes

	if (argc < 2){
		printf("USAGE ./server <int>\n");
		exit(-1);
	}

	int first_arg = atoi(argv[1]);

	printf("HELLO WORLD My int is %d\n" ,first_arg);

	initialize();

	for (int i= 0; i<MAXENTRIES; i++){
		for (int j =0; j< MAXTOPICS; j++){
			enqueue(&topic_queues[j]);
		}
	}


	for (int i = 0; i< MAXTOPICS; i++){
		//printf("outer %d", i);
		for (int j = 0; j< MAXENTRIES; j++){
			curtime = topic_queues[i].entries[j].timestamp.tv_sec;

			strftime(time_print_buffer,30,"%m-%d-%Y  %T.",localtime(&curtime));

			printf("hi from entry %d %s%ld\n", topic_queues[i].entries[j].entryNum,
			time_print_buffer, (long)topic_queues[i].entries[j].timestamp.tv_usec);
		}
	}


	exit_function();
}
////// END MAIN /////////////////////////////////////////////////////





