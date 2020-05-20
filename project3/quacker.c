// Project 3 "quacker"
// Christopher Wycoff

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

//////// START defines  ////////////////////////////////////////////////

#define URLSIZE 100
#define CAPSIZE 100
#define MAXENTRIES 4
#define MAXTOPICS 2
#define NUMPROXIES 10

//////// END defines  ////////////////////////////////////////////////


//////// START global variables ////////////////////////////////////////////////



struct node{
	struct node* next;
	char command_file[100];
};


struct pub_sub_queue{
	struct node* head;
	struct node* tail;
	int count;
};



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

//// biggest context data structure ////////////////////////////////////////////////
struct topic_queue topic_queues[MAXTOPICS]; // data structure holding the topic queues
//////////////////////////////////////////////////////////////////////////////////////

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


/// init the "get_entry" stuct ///////////////////////////////////////////////

struct topicEntry vessel_for_get_entry;

struct topicEntry vessel_for_enqueue;

struct pub_sub_queue sub_queue;

struct pub_sub_queue pub_queue;

//////////////////////////////////////////////////////////////////////////////

//////// END global variables ////////////////////////////////////////////////

//////// Begin initialize ////////////////////////////////////////////////////
void initialize() {
int i; //, j, k;

	//// initialize pub and sub command logic //////


	////////////////////////////////////////////////


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

//////// END initialize  ////////////////////////////////////////////////


/////////// START PUB/SUB COMMAND ENQUEUE and DEQUEUE ////////////////////////////////

void pub_sub_enqueue(struct pub_sub_queue* a_pub_sub_queue, char* command_string){
	struct node *new_node = malloc(sizeof(struct node));
	strcpy(new_node->command_file, command_string);
	new_node->next = NULL;
	if(a_pub_sub_queue->tail == NULL){
		a_pub_sub_queue->head = new_node;
	}
	else{
		a_pub_sub_queue->tail->next = new_node;
	}
	a_pub_sub_queue->tail = new_node;

}


char* pub_sub_dequeue(struct pub_sub_queue* a_pub_sub_queue){
	if (a_pub_sub_queue->head == NULL){
		return NULL;
	}
	else{
		char* result = a_pub_sub_queue->head->command_file;
		struct node* temp = a_pub_sub_queue->head;
		a_pub_sub_queue->head = a_pub_sub_queue->head->next;
		if (a_pub_sub_queue->head == NULL){
			a_pub_sub_queue->tail = NULL;
		}
		free(temp);
		return result;
	}
}

/////////// END PUB/SUB COMMAND ENQUEUE and DEQUEUE ////////////////////////////////

////// BEGIN ENQUEUE /////////////////////////////////////////////////////
int enqueue(struct topicEntry * a_topic_entry, struct topic_queue * a_topic_queue){

	printf("Calling enqueue\n");

	if (a_topic_queue->count >= MAXENTRIES){
		printf("queue full cannot enqueue\n");
		return -1;
	}
	// if this ^^^ does not happen there is room to enqueue

	// update entryNUM
	a_topic_queue->entries[a_topic_queue->head].entryNum = a_topic_queue->entry_number;

	gettimeofday(&a_topic_queue->entries[a_topic_queue->head].timestamp, NULL);

	// update photoURL
	strcpy(a_topic_queue->entries[a_topic_queue->head].photoURL, a_topic_entry->photoURL);

	strcpy(a_topic_queue->entries[a_topic_queue->head].photoCaption, a_topic_entry->photoCaption);

	//increment stuff

	a_topic_queue->count++;

	a_topic_queue->entry_number++;

	a_topic_queue->head = (a_topic_queue->head + 1) % MAXENTRIES ;

	//sleep(1);

	return 1;

}

////// END ENQUEUE /////////////////////////////////////////////////////

////// BEGIN ENQUEUE /////////////////////////////////////////////////////
int dequeue(struct topic_queue * a_topic_queue){
	printf("Calling dequeue\n");

	if (a_topic_queue->count <= 0){
		printf("queue empty cannot dequeue\n");
		return -1;
	}
	// if this ^^^ does not happen there is something to deque

	//decrement stuff

	a_topic_queue->count--;

	a_topic_queue->tail = (a_topic_queue->tail + 1) % MAXENTRIES ;

	//sleep(1);

	return 1;

}


////// END ENQUEUE /////////////////////////////////////////////////////

////// BEGIN GETENTRY /////////////////////////////////////////////////////

// does this not need to have an argument of the topic queue????

int getEntry(int lastEntry, struct topicEntry *a_topic_entry, int topic_id){
	//topic_queues[topic_id]
	//for (int i = 0; i < num )
	
	return 1;

}



////// END GETENTRY /////////////////////////////////////////////////////

////// BEGIN EXIT FUNCTION /////////////////////////////////////////////////////

void exit_function(){
	for (int i=0; i<MAXTOPICS; i++) {

		free(topic_queues[i].entries);

	}

	int keep_going = 1;

	while(keep_going){

		char * check_if_empty = pub_sub_dequeue(&pub_queue);

		if (check_if_empty == NULL){

				keep_going = 0;

			}
		else{
			//printf("dequing at exit: %s\n", check_if_empty);
		}
	}

	keep_going = 1;

	while(keep_going){

		char * check_if_empty = pub_sub_dequeue(&sub_queue);

		if (check_if_empty == NULL){

				keep_going = 0;

			}

		else{
			//printf("dequing at exit: %s\n", check_if_empty);
		}
	}	


	exit(0);

}
////// END EXIT FUNCTION /////////////////////////////////////////////////////


////// BEGIN MAIN /////////////////////////////////////////////////////
int main(int argc, char *argv[]){



	strcpy(vessel_for_enqueue.photoURL, "picture from the vessel");

	strcpy(vessel_for_enqueue.photoCaption, "caption from the vessel");

	time_t curtime; // for printing purposes

	if (argc < 2){
		printf("USAGE ./server <int>\n");
		exit(-1);
	}

	int first_arg = atoi(argv[1]);

	printf("HELLO WORLD My int is %d\n" ,first_arg);

	//initialize topic_queues and mutex locks
	initialize();

	//// TESTING AREA /////

	for (int i =0; i<4; i++){
	//test enqueue
	for (int i= 0; i<MAXENTRIES+3; i++){
		for (int j =0; j< MAXTOPICS; j++){
			enqueue(&vessel_for_enqueue, &topic_queues[j]);
		}
	}

	//test reading data
	for (int i = 0; i< MAXTOPICS; i++){
		//printf("outer %d", i);
		for (int j = 0; j< MAXENTRIES; j++){
			curtime = topic_queues[i].entries[j].timestamp.tv_sec;

			strftime(time_print_buffer,30,"%m-%d-%Y  %T.",localtime(&curtime));

			printf("hi from entry %d %s%ld\n", topic_queues[i].entries[j].entryNum,
			time_print_buffer, (long)topic_queues[i].entries[j].timestamp.tv_usec);
		}
	}

	//test dequeue
	for (int i= 0; i<MAXENTRIES+3; i++){
		for (int j =0; j< MAXTOPICS; j++){
			dequeue(&topic_queues[j]);
		}
	}

	//enque some more entries
	for (int i= 0; i<MAXENTRIES-1; i++){
		for (int j =0; j< MAXTOPICS; j++){
			enqueue(&vessel_for_enqueue, &topic_queues[j]);
		}
	}


	//test reading data
	for (int i = 0; i< MAXTOPICS; i++){
		//printf("outer %d", i);
		for (int j = 0; j< MAXENTRIES; j++){
			curtime = topic_queues[i].entries[j].timestamp.tv_sec;

			strftime(time_print_buffer,30,"%m-%d-%Y  %T.",localtime(&curtime));

			printf("hi from entry %d %s%ld\n", topic_queues[i].entries[j].entryNum,
			time_print_buffer, (long)topic_queues[i].entries[j].timestamp.tv_usec);

			printf("%s\n", topic_queues[i].entries[j].photoURL);
			printf("%s\n", topic_queues[i].entries[j].photoCaption);
		}
	}
	///sleep(2);
	}

	char* test_char_pp[] = {"hello Wolrd", "I am a c g", "this.txt"};


	for(int i = 0; i<3; i++){
		pub_sub_enqueue(&pub_queue, test_char_pp[i]);
		pub_sub_enqueue(&sub_queue, test_char_pp[i]);
		printf("%s\n",test_char_pp[i]);
	}

	//// END TESTING AREA /////


	exit_function();
}
////// END MAIN /////////////////////////////////////////////////////





