#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include "command.h"


int read_string(char* the_string){
	char current_char = the_string[0];
	int counter = 0;
	while(current_char != '\0'){
		counter+=1;
		current_char = the_string[counter];
	}
	return counter;
}


void listDir(){
	//printf("running listDir\n");
	struct dirent *directory_entity;

	char * entity_string;

	int entity_len;

	DIR *this_directory = opendir(".");

	char * error_message = "could_not_open_directory";

	int error_message_len = read_string(error_message);

	if (this_directory == NULL){
		write(1,error_message,error_message_len);
		return;
	}
	while((directory_entity = readdir(this_directory)) != NULL){
		entity_string = directory_entity->d_name;
		entity_len = read_string(entity_string);
		write(1," ",1);
		write(1,entity_string,entity_len);
	}
	write(1,"\n",1);
	closedir(this_directory);
	return;

} 

/*for the cat command*/
void displayFile(char *filename){
	char the_buffer;

	int char_int;

	int source_file = open(filename, O_RDONLY);

	int stop_counter = 0;
	while((char_int = read(source_file, &the_buffer, 1) != 0) && stop_counter < 1000000 ){
		write(1,&the_buffer,1);
		stop_counter += 1;
	}
	close(source_file);

}



void lfcat()
{
	// Define your variables here

	// Get the current directory
	struct dirent *directory_entity;

	char * entity_string;

	int entity_len;

	char the_buffer;

	int char_int;

	char this_dir[2048];

	getcwd(this_dir, 2048);

	DIR *this_directory = opendir(this_dir);

	char * error_message = "could_not_open_directory";

	int error_message_len = read_string(error_message);

	if (this_directory == NULL){
		write(1,error_message,error_message_len);
		return;
	}
	while((directory_entity = readdir(this_directory)) != NULL){
		entity_string = directory_entity->d_name;
		entity_len = read_string(entity_string);

		if (!strcmp(entity_string,"1_poem.txt") || !strcmp(entity_string,"2_lyrics.txt") 
			||!strcmp(entity_string,"3_DE_Code.py")){
			write(1,"File: ",6);
			write(1,entity_string,entity_len);
			write(1,"\n",1);



			for(int i = 0; i<80; i++){
				write(1,"-",1);
			}
			
			write(1,"\n",1);

				

			int source_file = open(entity_string, O_RDONLY);

			int stop_counter = 0;
			while((char_int = read(source_file, &the_buffer, 1) != 0) && stop_counter < 1000000 ){
				write(1,&the_buffer,1);
				stop_counter += 1;
			}
			close(source_file);
			write(1,"\n",1);



		}// end if

		
	}

	write(1,"\n",1);
	closedir(this_directory);
	return;

	// Open the dir using opendir()

	// use a while loop to read the dir

		// Hint: use an if statement to skip any names that are not readable files (e.g. ".", "..", "main.c", "a.out", "output.txt"

			// Open the file

			// Read in each line using getline()
				// Write the line to stdout

			// write 80 "-" characters to stdout

			// close the read file and free/null assign your line buffer

	//close the directory you were reading from using closedir()
}
