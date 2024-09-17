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
	DIR *dirP;
	int fileCount=0;	//counter for readable files, return this

	//handle no input path error case
	if(inputPath == NULL){	
		inputPath=getcwd(NULL,0);//buf to NULL and size to 0 means dynamic sizing
		printf("No path specified, working from current dir: <%s>\n",inputPath);
	}

	if((dirP=opendir(inputPath))==NULL){
		fprintf(stderr,"Could not open directory <%s>\n",strerror(errno));
		return -errno;
	}

	//recursive while block, goes until entry is NULL or it finds a directory, then recurses to either return a value or read another directory
	while((entry = readdir(dirP)) != NULL){

		//skip over . and .. directories to avoid infinite recursion
		if(strcmp(entry->d_name, ".")==0 || strcmp(entry->d_name, "..")==0){
			continue;
		}
		
		char dirPath[PATH_MAX];
		snprintf(dirPath,sizeof(dirPath),"%s/%s", inputPath, entry->d_name);
	
		//if block to check what the current file is
		if(stat(dirPath,s)==0){
			//if it is a file
			if(S_ISREG (s->st_mode)){
				printf("%s is a regular file.\n",entry->d_name);
				if(access(dirPath,R_OK)==0){
					printf("Can read <%s>\n",entry->d_name);
					fileCount++;
				}else{
					printf("Cannot read <%s>\n",entry->d_name);
				}
			//if it is a directory
			}else if(S_ISDIR(s->st_mode)){
				printf("%s is a directory.\n",entry->d_name);

				fileCount += readable(dirPath);

				if(access(dirPath,F_OK)==0){
					printf("Can access dir: <%s>\n",entry->d_name);
					
				}else{
					printf("Unable to access dir: <%s>\n",entry->d_name);
				}	
			//if it is anything else (special etc.)
			}else{
				printf("%s is a special file.\n",entry->d_name);
			}
		}else{
			printf("%s does not exist or is not reachable.\n",entry->d_name);
		}	
		
	}
	closedir(dirP);
	printf("Readable files found: <%d>\n",fileCount);
	return fileCount;
}

int main(int argc, char *argv[]){
	char *path=argv[1];
	readable(path);
}
