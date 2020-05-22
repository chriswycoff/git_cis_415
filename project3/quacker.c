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
#define MAXENTRIES 5
#define MAXTOPICS 2
#define NUMPROXIES 10
#define TEST_DELTA 2

volatile int DONE = 0;
volatile int pub_threads_left = NUMPROXIES;
volatile int sub_threads_left = NUMPROXIES;


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
	int max;
	int	count, head, tail;
	struct topicEntry *entries;
};

//// biggest context data structure ////////////////////////////////////////////////
struct topic_queue topic_queues[MAXTOPICS]; // data structure holding the topic queues
pthread_mutex_t topic_queue_mutexes[MAXTOPICS]; // mutexes for the topic store
//////////////////////////////////////////////////////////////////////////////////////

pthread_t pubs[NUMPROXIES];		// thread ID for publishers
pthread_t subs[NUMPROXIES];		// thread ID for subscribers
pthread_attr_t attr; 

/// cleanup pthread ///
pthread_t cleanup_thread;
pthread_attr_t clean_attr; 



// thread pool mutexs

pthread_mutex_t sub_queue_mutex;
pthread_mutex_t pub_queue_mutex;

//// pthread condition variables ///////
pthread_cond_t sub_queue_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t pub_queue_cond = PTHREAD_COND_INITIALIZER;

////////////////////////////////////////

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

int test_int = 0;

pthread_mutex_t int_test_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t done_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t pub_left_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t sub_left_mutex = PTHREAD_MUTEX_INITIALIZER;


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
		pthread_mutex_init(&(topic_queue_mutexes[i]), NULL);
	//    sem_init(&full[i], 0, 0);
	//    sem_init(&empty[i], 0, BUFFERSIZE);
	}
	pthread_mutex_init(&pub_queue_mutex, NULL);
	pthread_mutex_init(&sub_queue_mutex, NULL);


	pthread_attr_init(&attr);
	pthread_attr_init(&clean_attr);
	//pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

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


char* pub_sub_dequeue(struct pub_sub_queue* a_pub_sub_queue, char* command_slot){
	
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
		if (result != NULL ){
			strcpy(command_slot, result);
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
	/*
	FOR REFERENCE
	struct topic_queue{
		int entry_number;
		int	count, head, tail;
		struct topicEntry *entries;
	};

	struct topicEntry{
		int entryNum;
		struct timeval timestamp;
		int pubId;
		char photoURL[URLSIZE];
		char photoCaption[CAPSIZE];
	};

	*/

	struct topic_queue * specific_queue = &topic_queues[topic_id];

	if (specific_queue->count < 0){
		printf("ERROR \n");
		return -1;
	}

	if (specific_queue->count == 0){
		return 0;
	}
	/// topic_queue not empty so continue

	int keep_going = 1;
	int the_count = specific_queue->count;
	int index = specific_queue->tail;

	int counter = 0;

	while(keep_going){
		if (specific_queue->entries[index].entryNum == lastEntry+1){
			/////copy the topic entry////
			a_topic_entry->entryNum = specific_queue->entries[index].entryNum;
			a_topic_entry->timestamp = specific_queue->entries[index].timestamp;
			a_topic_entry->pubId = specific_queue->entries[index].pubId;
			strcpy(specific_queue->entries[index].photoURL, a_topic_entry->photoURL);
			strcpy(specific_queue->entries[index].photoCaption, a_topic_entry->photoCaption);
			
			return 1;
		}

		if (specific_queue->entries[index].entryNum > lastEntry+1){
			/////copy the topic entry////
			a_topic_entry->entryNum = specific_queue->entries[index].entryNum;
			a_topic_entry->timestamp = specific_queue->entries[index].timestamp;
			a_topic_entry->pubId = specific_queue->entries[index].pubId;
			strcpy(specific_queue->entries[index].photoURL, a_topic_entry->photoURL);
			strcpy(specific_queue->entries[index].photoCaption, a_topic_entry->photoCaption);
			
			return a_topic_entry->entryNum;
		}


		index = (index + 1 ) % MAXENTRIES;

		// loop breaking logic
		counter += 1;

		if (counter > the_count){
			keep_going = 0;
		}

	}
	
	return 0;

}



////// END GETENTRY /////////////////////////////////////////////////////

////// BEGIN EXIT FUNCTION /////////////////////////////////////////////////////

void exit_function(){
	char command_slot[100];
	for (int i=0; i<MAXTOPICS; i++) {

		free(topic_queues[i].entries);

	}

	int keep_going = 1;

	while(keep_going){

		char * check_if_empty = pub_sub_dequeue(&pub_queue,command_slot);

		if (check_if_empty == NULL){

				keep_going = 0;

			}
		else{
			printf("dequing pubs at exit \n");
		}
	}

	keep_going = 1;

	while(keep_going){

		char * check_if_empty = pub_sub_dequeue(&sub_queue,command_slot);

		if (check_if_empty == NULL){

				keep_going = 0;

			}

		else{
			printf("dequing subs at exit \n");
		}
	}	


	exit(0);

}

////// END EXIT FUNCTION //////////////////////////////////////////////////////


///// TESTING ROUTINES /////////////////////////////////////////////////////////



/////////////END TESTING ROUTINES///////////////////////////////////////////////

////// BEGIN PUBLISHER Handler FUNCTION ////////////////////////////////////////

void handle_publisher(char *command_file){
	printf("Handler being called %s\n", command_file);
}
////// END PUBLISHER Handler FUNCTION ////////////////////////////////////////


////// BEGIN PUBLISHER THREAD FUNCTION ////////////////////////////////////////
void * publisher(void * params){

	struct threadargs* my_arguments = (struct threadargs *)params;


	int keep_going = 1;
	char command_slot[100];
	
	while(keep_going){
		if (my_arguments->id % 3 == 0){
			sleep(2); // to simulate some threads sleeping
		}
		printf("HELLO FROM PUB THREAD %d \n",my_arguments->id);
		pthread_mutex_lock(&pub_queue_mutex);
		pthread_cond_wait(&pub_queue_cond, &pub_queue_mutex);
		printf("UNLOCKING and grabbing\n");
		char * check_if_empty = pub_sub_dequeue(&pub_queue, command_slot);
		if (check_if_empty != NULL){
			printf("Grabbed pub command: %s\n", command_slot);
			// call handler //
			handle_publisher(command_slot);
			// end call handler
		}
		pthread_mutex_unlock(&pub_queue_mutex);

		pthread_mutex_lock(&done_mutex);
		if (DONE == 1){
			printf("Thread: %d GOT HERE\n",my_arguments->id);
			keep_going = 0;
		}
		pthread_mutex_unlock(&done_mutex);
	}

	pthread_mutex_lock(&pub_left_mutex);
	pub_threads_left -= 1;
	pthread_mutex_unlock(&pub_left_mutex);
	return NULL;

}

////// END PUBLISHER THREAD FUNCTION //////////////////////////////////////////


////// BEGIN SUBSCRIBER Handler FUNCTION ////////////////////////////////////////

void handle_subscriber(char* command_file){
	printf("Handler being called %s\n", command_file);
}
////// END SUBSCRIBER Handler FUNCTION ////////////////////////////////////////

////// BEGIN SUBSCRIBER THREAD FUNCTION ///////////////////////////////////////
void * subscriber(void * params){

struct threadargs* my_arguments = (struct threadargs *)params;

	int keep_going = 1;
	char command_slot[100];
	
	while(keep_going){
		if (my_arguments->id % 3 == 0){
			sleep(2); // to simulate some threads sleeping
		}
		printf("HELLO FROM SUB THREAD %d \n",my_arguments->id);
		pthread_mutex_lock(&sub_queue_mutex);
		pthread_cond_wait(&sub_queue_cond, &sub_queue_mutex);
		printf("UNLOCKING and grabbing\n");
		char * check_if_empty = pub_sub_dequeue(&sub_queue, command_slot);
		if (check_if_empty != NULL){
			printf("Grabbed sub command: %s\n", command_slot);
			// call handler //
			handle_subscriber(command_slot);
			// end call handler
		}
		pthread_mutex_unlock(&sub_queue_mutex);

		// after handler check if done
		pthread_mutex_lock(&done_mutex);
		if (DONE == 1){
			printf("Thread: %d GOT HERE\n",my_arguments->id);
			keep_going = 0;
		}
		pthread_mutex_unlock(&done_mutex);
	}

	pthread_mutex_lock(&sub_left_mutex);
	sub_threads_left -= 1;
	pthread_mutex_unlock(&sub_left_mutex);

	return NULL;
	
}

////// END SUBSCRIBER THREAD FUNCTION /////////////////////////////////////////

////// BEGIN ClEANUP THREAD FUNCTION ///////////////////////////////////////

void * cleanup_thread_function(void * params){
	int number_entried_dequeue;

	while(!DONE){ 
		//sleep(2);
		sleep(1);
		for (int i=0; i<MAXTOPICS; i++) {
			number_entried_dequeue = 0;
			pthread_mutex_lock(&topic_queue_mutexes[i]);
			/// determine how many to dequeue ///
			/*
			FOR REFERENCE
			struct topic_queue{
				int entry_number;
				int	count, head, tail;
				struct topicEntry *entries;
			};

			struct topicEntry{
				int entryNum;
				struct timeval timestamp;
				int pubId;
				char photoURL[URLSIZE];
				char photoCaption[CAPSIZE];
			};

			*/
			struct timeval cleanup_time_stamp;

			struct topic_queue * specific_queue = &topic_queues[i];

			if (specific_queue->count < 0){
				printf("ERROR \n");
				return NULL;
			}

				if (specific_queue->count != 0){
					// tho
				
					/// topic_queue not empty so continue

					int keep_going = 1;
					int the_count = specific_queue->count;
					int index = specific_queue->tail;
					int counter = 0;

					while(keep_going){
						gettimeofday(&cleanup_time_stamp, NULL); 
						printf("time here %d \n",cleanup_time_stamp.tv_sec);
						int how_old_in_seconds = cleanup_time_stamp.tv_sec - specific_queue->entries[index].timestamp.tv_sec;
						printf("this entry is %d seconds old\n", how_old_in_seconds);
						// for reference int dequeue(struct topic_queue * a_topic_queue)
						if (how_old_in_seconds > TEST_DELTA){
							dequeue(specific_queue);
						}

						/*
						if (specific_queue->entries[index].entryNum == lastEntry+1){
							/////copy the topic entry////
							a_topic_entry->entryNum = specific_queue->entries[index].entryNum;
							a_topic_entry->timestamp = specific_queue->entries[index].timestamp;
							a_topic_entry->pubId = specific_queue->entries[index].pubId;
							strcpy(specific_queue->entries[index].photoURL, a_topic_entry->photoURL);
							strcpy(specific_queue->entries[index].photoCaption, a_topic_entry->photoCaption);
							
							return 1;
						}

						if (specific_queue->entries[index].entryNum > lastEntry+1){
							/////copy the topic entry////
							a_topic_entry->entryNum = specific_queue->entries[index].entryNum;
							a_topic_entry->timestamp = specific_queue->entries[index].timestamp;
							a_topic_entry->pubId = specific_queue->entries[index].pubId;
							strcpy(specific_queue->entries[index].photoURL, a_topic_entry->photoURL);
							strcpy(specific_queue->entries[index].photoCaption, a_topic_entry->photoCaption);
							
							return a_topic_entry->entryNum;
						}
						*/


						index = (index + 1 ) % MAXENTRIES;

						// loop breaking logic
						counter += 1;

					if (counter > the_count){
						keep_going = 0;
					}

				}
			}

			pthread_mutex_unlock(&topic_queue_mutexes[i]);
			//sched_yield();
		}
	}


	return NULL;
}

////// END CLEANUP THREAD FUNCTION /////////////////////////////////////////



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


	//// BEGIN TESTING AREA /////
	int entries_to_get[]= {0, 2, 5, 16, 20};
	int num_entries_to_get = 5;


	/////////////////////////////


	//initialize topic_queues and mutex locks
	initialize();

	//// SPIN UP THREADS ////

	char* test_char_pp[] = {"command_file1.txt", "command_file2.txt", "command_file1.txt"};

	for (int i=0; i < NUMPROXIES; i++){
		pubargs[i].id = i;
		subargs[i].id = i;
		pthread_create(&pubs[i], &attr, publisher, (void *) &pubargs[i]);
		pthread_create(&subs[i], &attr, subscriber, (void *) &subargs[i]);

	}
	pthread_create(&cleanup_thread, &clean_attr, cleanup_thread_function, NULL);
	
	sleep(1);

	for(int i = 0; i<3; i++){
		//printf("Main SERVER Unlocking\n");
		// ADD COMMANDS TO QUEUE
		pthread_mutex_lock(&pub_queue_mutex);
		pub_sub_enqueue(&pub_queue, test_char_pp[i%3]);
		pthread_mutex_unlock(&pub_queue_mutex);

		pthread_mutex_lock(&sub_queue_mutex);
		pub_sub_enqueue(&sub_queue, test_char_pp[i%3]);
		pthread_mutex_unlock(&sub_queue_mutex);

		///////////// SIGNAL TREADS ////////////

		pthread_mutex_lock(&pub_queue_mutex);
		pthread_cond_signal(&pub_queue_cond);
		pthread_mutex_unlock(&pub_queue_mutex);

		pthread_mutex_lock(&sub_queue_mutex);
		pthread_cond_signal(&sub_queue_cond);
		pthread_mutex_unlock(&sub_queue_mutex);
		
		//printf("%s\n",test_char_pp[i]);
	}

	/*
	// OVERARCHING LOOP HERE /////
	for (int i =0; i<3; i++){

		//test enqueue
		for (int i= 0; i<MAXENTRIES+3; i++){
			for (int j =0; j< MAXTOPICS; j++){
				enqueue(&vessel_for_enqueue, &topic_queues[j]);
			}
		}

		// test getEntry ////
		int get_entry_return = -4;
		for (int i = 0; i< MAXTOPICS; i++){ 
			for (int j =0; j< num_entries_to_get; j++){
				get_entry_return = getEntry(entries_to_get[j], &vessel_for_get_entry, i);
				printf("looking for entry: %d \n", entries_to_get[j] + 1);
				printf("the result of get entry was: %d \n", get_entry_return);
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
		sleep(3);
		//pthread_create(&cleanup_thread, &clean_attr, cleanup_thread_function, NULL);


		//test reading data
		for (int i = 0; i< MAXTOPICS; i++){
			//printf("outer %d", i);
			for (int j = 0; j< MAXENTRIES; j++){
				curtime = topic_queues[i].entries[j].timestamp.tv_sec;

				strftime(time_print_buffer,30,"%m-%d-%Y  %T.",localtime(&curtime));

				printf("hi from entry %d %s%ld\n", topic_queues[i].entries[j].entryNum,
				time_print_buffer, (long)topic_queues[i].entries[j].timestamp.tv_usec);

				//printf("%s\n", topic_queues[i].entries[j].photoURL);
				//printf("%s\n", topic_queues[i].entries[j].photoCaption);
			}
		}
		///sleep(2);
	}
	*/


	/*
	for (int i=0; i < NUMPROXIES; i++){
		pubargs[i].id = i;
		subargs[i].id = i;
		pthread_create(&pubs[i], &attr, publisher, (void *) &pubargs[i]);
		pthread_create(&subs[i], &attr, subscriber, (void *) &subargs[i]);

	}
	pthread_create(&cleanup_thread, &clean_attr, cleanup_thread_function, NULL);
	
	sleep(1);

	

	for(int i = 0; i<3; i++){
		//printf("Main SERVER Unlocking\n");
		// ADD COMMANDS TO QUEUE
		pthread_mutex_lock(&pub_queue_mutex);
		pub_sub_enqueue(&pub_queue, test_char_pp[i%3]);
		pthread_mutex_unlock(&pub_queue_mutex);

		pthread_mutex_lock(&sub_queue_mutex);
		pub_sub_enqueue(&sub_queue, test_char_pp[i%3]);
		pthread_mutex_unlock(&sub_queue_mutex);

		///////////// SIGNAL TREADS ////////////

		pthread_mutex_lock(&pub_queue_mutex);
		pthread_cond_signal(&pub_queue_cond);
		pthread_mutex_unlock(&pub_queue_mutex);

		pthread_mutex_lock(&sub_queue_mutex);
		pthread_cond_signal(&sub_queue_cond);
		pthread_mutex_unlock(&sub_queue_mutex);
		
		//printf("%s\n",test_char_pp[i]);
	}
	*/

	//// END TESTING AREA ////
	
	//printf("here\n");
	sleep(5);
	pthread_mutex_lock(&done_mutex);
	DONE = 1;
	pthread_mutex_unlock(&done_mutex);
	

	//int pubs_left = sub_queue.count;

	/*
	for(int i = 0; i<NUMPROXIES; i++){
		pthread_mutex_lock(&pub_queue_mutex);
		pthread_cond_signal(&pub_queue_cond);
		pthread_mutex_unlock(&pub_queue_mutex);
	}
	*/
	/*
	while(pubs_left){
		pubs_left = sub_queue.count;
		sleep(1);
	}
	*/

	////////////// START PUB THREAD CLEANUP /////////////////
	sleep(1);
	int times_executed = 0;
	while(pub_threads_left > 0){
		times_executed+=1;
		pthread_mutex_lock(&pub_queue_mutex);
		pthread_cond_signal(&pub_queue_cond);
		pthread_mutex_unlock(&pub_queue_mutex);
		//sleep(1);
	}

	for (int i=0; i < NUMPROXIES; i++){
		//pthread_cancel(pubs[i]);
		//printf("here : %d\n", i);
		pthread_join(pubs[i], NULL);
	}

	////////////// END PUB THREAD CLEANUP /////////////////

	////////////// START SUB THREAD CLEANUP /////////////////
	
	sleep(1);
	while(sub_threads_left > 0){
		times_executed+=1;
		pthread_mutex_lock(&sub_queue_mutex);
		pthread_cond_signal(&sub_queue_cond);
		pthread_mutex_unlock(&sub_queue_mutex);
		//sleep(1);
	}

	for (int i=0; i < NUMPROXIES; i++){
		//pthread_cancel(pubs[i]);
		//printf("here : %d\n", i);
		pthread_join(subs[i], NULL);
	}
	
	pthread_join(cleanup_thread, NULL);

	////////////// END SUB THREAD CLEANUP /////////////////

	printf("times_executed: %d\n", times_executed); //wow that is a lot
	exit_function();
}
////// END MAIN /////////////////////////////////////////////////////





