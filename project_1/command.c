/*
command.c
implementation of the different commands

*/

#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"


/*for the ls command*/
void listDir(){
	 printf("running listDir\n");
} 

/*for the pwd command*/
void showCurrentDir(){
	printf("running showCurrentDir\n");
}

 /*for the mkdir command*/
void makeDir(char *dirName){
	printf("running makeDir: %s\n", dirName);
}

/*for the cd command*/
void changeDir(char *dirName){
	printf("running changeDir: %s\n", dirName);

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









