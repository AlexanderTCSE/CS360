/***********************************************************************
name: Alexander Tattersall
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
	char *cwd = NULL;

	//handle no input path error case
	if(inputPath == NULL){	
		cwd=getcwd(NULL,0);//buf to NULL and size to 0 means dynamic sizing
		if(cwd==NULL){
			fprintf(stderr,"Cannot open NULL dir <%s>\n",strerror(errno));
			return -errno;
		}
		inputPath=cwd;
		//printf("No path specified, working from current dir: <%s>\n",inputPath);
	}

	if(lstat(inputPath,s)==0){
		if(S_ISREG(s->st_mode)){
			if(access(inputPath,R_OK)==0){
				free(cwd);
				return 1;
			}else{
				fprintf(stderr,"Error: File <%s> is not readable: %s\n",inputPath,strerror(errno));
				free(cwd);
				return 0;
			}
		}
	}

	//more checks if initial dir exists
	if(access(inputPath,F_OK) != 0){
		fprintf(stderr,"Error: Provided dir <%s> does not exist: %s\n",inputPath,strerror(errno));
		free(cwd);
		return -errno;
	}

	//check if it is accessible
	if(access(inputPath,R_OK) != 0){
		fprintf(stderr,"Error: Provided dir <%s> is inaccessible: %s\n",inputPath,strerror(errno));
		free(cwd);
		return -errno;
	}

	//attempt to open directory
	if((dirP=opendir(inputPath))==NULL){
		fprintf(stderr,"Error: Could not open directory <%s: %s>\n",inputPath,strerror(errno));
		free(cwd);
		return 0;	
	}

	//recursive while block, goes until entry is NULL or it finds a directory, then recurses to either return a value or read another directory
	while((entry = readdir(dirP)) != NULL){

		//skip over . and .. directories to avoid infinite recursion
		if(strcmp(entry->d_name, ".")==0 || strcmp(entry->d_name, "..")==0){
			continue;
		}
		
		char dirPath[PATH_MAX];
		sprintf(dirPath,"%s/%s", inputPath, entry->d_name);
	
		//if block to check what the current file is
		if(lstat(dirPath,s)==0){
			//if it is a file
			if(S_ISREG (s->st_mode)){
				//printf("%s is a regular file.\n",entry->d_name);
				if(access(dirPath,R_OK)==0){
					//printf("Can read <%s>\n",entry->d_name);
					fileCount++;
				}else{
					//printf("Cannot read <%s>\n",entry->d_name);
					continue;
				}
			//if it is a directory
			}else if(S_ISDIR(s->st_mode)){
				//printf("%s is a directory.\n",entry->d_name);
				if(access(dirPath,R_OK)==0){
					//printf("Can access dir: <%s>\n",entry->d_name);
					int subDirCount = readable(dirPath);				
					if(subDirCount < 0){
						continue;
					}
					fileCount += subDirCount;
				}else{
					//printf("Unable to access dir: <%s>\n",entry->d_name);
					continue;	
				}	
			//if it is anything else (special etc.)
			}else{
				//printf("%s is a special file.\n",entry->d_name);
				continue;
			}
		}else{
			//printf("%s does not exist or is not reachable.\n",entry->d_name);
			continue;
		}	
	}
	closedir(dirP);
	free(cwd);
	//printf("Readable files found in dir: <%d>\n",fileCount);
	return fileCount;
}


int main(int argc, char *argv[]){
	char *path=argv[1];
	int total=readable(path);
	printf("Total readable files found: %d\n",total);
}

