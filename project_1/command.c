/*
command.c
implementation of the different commands
*/

/*
Questions:
mkdir: path required?? or just cwd?
path relative??
any forking required?
cd relative

*/

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

/*for the ls command*/
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

/*for the pwd command*/
void showCurrentDir(){
	char this_dir[2048];
	getcwd(this_dir, 2048);
	int the_string_size_max = read_string(this_dir);
	write(1,this_dir,the_string_size_max);
	write(1,"\n",1);
}

 /*for the mkdir command*/
void makeDir(char *dirName){

	//printf("running makeDir: %s\n", dirName);
	char this_dir[2048];
	getcwd(this_dir, 2048);
	int the_string_size_max = read_string(this_dir);

	int char_counter = 0;
	char character_to_add = dirName[char_counter];

	this_dir[the_string_size_max] = '/';
	the_string_size_max += 1;

	while(character_to_add != '\0'){
		this_dir[the_string_size_max] = character_to_add;

		char_counter += 1;
		character_to_add = dirName[char_counter];

		the_string_size_max += 1;
	}

	this_dir[the_string_size_max] = '\0';


	mkdir(this_dir,0777);

}

/*for the cd command*/
void changeDir(char *dirName){
	chdir(dirName);
}

/*for the cp command*/
void copyFile(char *sourcePath, char *destinationPath){
	printf("running copyFile: %s -> %s \n", sourcePath, destinationPath);
}

/*for the mv command*/
void moveFile(char *sourcePath, char *destinationPath){
	printf("running moveFile: %s -> %s \n", sourcePath, destinationPath);
}
/*for the rm command*/
void deleteFile(char *filename){
	printf("running deleteFile: %s \n", filename);

}

/*for the cat command*/
void displayFile(char *filename){
	printf("running displayFile: %s \n", filename);


}









