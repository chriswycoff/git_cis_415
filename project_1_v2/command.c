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

scr vs destination specificity?

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

void add_slash_and_name(char* name, char* name_to_add){

	int the_string_size_max = read_string(name);

	int char_counter = 0;
	char character_to_add = name_to_add[char_counter];

	name[the_string_size_max] = '/';
	the_string_size_max += 1;

	while(character_to_add != '\0'){
		name[the_string_size_max] = character_to_add;

		char_counter += 1;
		character_to_add = name_to_add[char_counter];

		the_string_size_max += 1;
	}

	name[the_string_size_max] = '\0';
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
	/*
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

*/
	mkdir(dirName,0777);

}

/*for the cd command*/
void changeDir(char *dirName){
	int success = 0;
	success = chdir(dirName);

	char * error_message = "could_not_open_directory1";

	int error_message_len = read_string(error_message);

	if (success == -1){
		write(1,error_message,error_message_len);
		return;
	}
}

/*for the cp command*/
void copyFile(char *sourcePath, char *destinationPath){

	char * error_message1 = "file not found";
	char * error_message2 = "file not able to open";
	int error_message_len1 = read_string(error_message1);
	int error_message_len2 = read_string(error_message2);

	int len_of_source = read_string(sourcePath);

	int len_of_dest = read_string(destinationPath);

	char the_buffer;

	int char_int;

	struct stat sb;

	char copy_of_destination_path[2048];
	///

	char parsed_name[2048];

	int relative_or_not = 0; 
	
	for(int i = 0; i<len_of_source; i++){
		if (sourcePath[i] == '/'){
			relative_or_not = i;
		}
	}

	if (relative_or_not){
		int j = 0; 
		for(int i = relative_or_not; i<len_of_source; i++){
			parsed_name[j] = sourcePath[i];
			j += 1;
		}
		parsed_name[j] = '\0';
	}
	else{
		parsed_name[0]= '/';
		int counter_for_source = 0;
		for(int i = 1; i < len_of_source+1; i++){
			parsed_name[i] = sourcePath[counter_for_source];
			counter_for_source += 1;
		}
		parsed_name[len_of_source+1] = '\0';

	}

	//printf("1%s\n", parsed_name);

	int len_of_parsed_name = read_string(parsed_name);

	///
	for(int i = 0; i < len_of_dest; i++){
			copy_of_destination_path[i] = destinationPath[i];
		}
		
	for(int i = 0; i < len_of_parsed_name; i++){
		copy_of_destination_path[len_of_dest] = parsed_name[i];
		len_of_dest+= 1;
	}
	copy_of_destination_path[len_of_dest] = '\0';

	//printf("2%s\n", copy_of_destination_path);

	///

	if (stat(destinationPath, &sb) == 0 && S_ISDIR(sb.st_mode))
	{
    // printf("yup directory\n");
    destinationPath = copy_of_destination_path;

	}

	///
	int source_file = open(sourcePath, O_RDONLY);
	if (source_file == -1){
		write(1,error_message1,error_message_len1);
		return;
	}
	
	int destination_file = open(destinationPath, O_CREAT|O_RDWR, 0777);
		if (destination_file == -1){
		close(source_file);
		write(1,error_message2,error_message_len2);
		return;
	}

	int stop_counter = 0;
	while((char_int = read(source_file, &the_buffer, 1) != 0)){
		write(destination_file,&the_buffer,1);
		stop_counter += 1;
	}
	close(source_file);
	close(destination_file);
	///
}

/*for the mv command*/
void moveFile(char *sourcePath, char *destinationPath){
	////

	int len_of_source = read_string(sourcePath);

	int len_of_dest = read_string(destinationPath);


	struct stat sb;

	char copy_of_destination_path[2048];
	///

	char parsed_name[2048];

	int relative_or_not = 0; 
	
	for(int i = 0; i<len_of_source; i++){
		if (sourcePath[i] == '/'){
			relative_or_not = i;
		}
	}

	if (relative_or_not){
		int j = 0; 
		for(int i = relative_or_not; i<len_of_source; i++){
			parsed_name[j] = sourcePath[i];
			j += 1;
		}
		parsed_name[j] = '\0';
	}
	else{
		parsed_name[0]= '/';
		int counter_for_source = 0;
		for(int i = 1; i < len_of_source+1; i++){
			parsed_name[i] = sourcePath[counter_for_source];
			counter_for_source += 1;
		}
		parsed_name[len_of_source+1] = '\0';

	}

	//printf("1%s\n", parsed_name);

	int len_of_parsed_name = read_string(parsed_name);

	///
	for(int i = 0; i < len_of_dest; i++){
			copy_of_destination_path[i] = destinationPath[i];
		}

		
	for(int i = 0; i < len_of_parsed_name; i++){
		copy_of_destination_path[len_of_dest] = parsed_name[i];
		len_of_dest+= 1;
	}
	copy_of_destination_path[len_of_dest] = '\0';

	//printf("2%s\n", copy_of_destination_path);

	///

	if (stat(destinationPath, &sb) == 0 && S_ISDIR(sb.st_mode))
	{
    //printf("yup directory\n");
    destinationPath = copy_of_destination_path;

	}

	//printf("11: %s\n",destinationPath);

	///


	////
	int success = 0;

	success = rename(sourcePath,destinationPath);

	char * error_message = "could_not_open_directory";

	int error_message_len = read_string(error_message);

	if (success == -1){
		write(1,error_message,error_message_len);
		return;
	}

}

/*for the rm command*/
void deleteFile(char *filename){
	unlink(filename);

}

/*for the cat command*/
void displayFile(char *filename){
	char the_buffer;

	int char_int;

	int source_file = open(filename, O_RDONLY);
	if (source_file == -1){
		printf("file not found\n");
		return;
		}

	int stop_counter = 0;
	while((char_int = read(source_file, &the_buffer, 1) != 0)){
		write(1,&the_buffer,1);
		stop_counter += 1;
	}
	close(source_file);

}









