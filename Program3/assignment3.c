/***********************************************************************
name:
	readable -- recursively count readable files.
description:	
	See CS 360 Files and Directories lectures for details.
***********************************************************************/

/* Includes and definitions */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/stat.h>
#include<dirent.h>
#include<unistd.h>

/**********************************************************************
Take as input some path. Recursively count the files that are readable.
If a file is not readable, do not count it. Return the negative value
of any error codes you may get. However, consider what errors you want
to respond to.
**********************************************************************/


int readable(char* inputPath) {
	struct stat area, *s = &area;
	struct dirent *entry;
	char *dirP;	
	int fileCount=0;	//counter for readable files, return this

	//handle no input path error case
	if(inputPath == NULL){	
		inputPath=getcwd(NULL,0);//buf to NULL and size to 0 means dynamic sizing
		printf("No path specified, working from current dir: <%s>\n",inputPath);
	}

	//if block to check what the current file is
	if(stat(inputPath,s)==0){
		if(S_ISREG (s->st_mode)){
			printf("%s is a regular file.\n",inputPath);
			if(access(inputPath,R_OK)==0){
				printf("Can read %s.\n",inputPath);
				fileCount++;
			}else{
				printf("Cannot read %s.\n",inputPath);
			}
		}else if(S_ISDIR(s->st_mode)){
			printf("%s is a directory.\n",inputPath);
			if(access(inputPath,F_OK)==0){
				printf("Can access dir: %s.\n",inputPath);
					
			}else{
				printf("Unable to access dir: %s.\n",inputPath);
			}	
		}else{
			printf("%s is a special file.\n",inputPath);
		}
	}else{
		printf("%s does not exist or is not reachable.\n",inputPath);
	}	
	return 0;
}

int main(int argc, char *argv[]){
	char *path=argv[1];
	readable(path);
}
