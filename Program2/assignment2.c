/***********************************************************************
name:
	lineNum -- see if a word is in the online dictionary
description:	
	See CS 360 IO lecture for details.
***********************************************************************/

#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

/**********************************************************************
Search for given word in dictionary using file descriptor fd.
Return the line number the word was found on, negative of the last line searched
if not found, or the error number if an error occurs.
**********************************************************************/
int lineNum(char *dictionaryName, char *word, int length) {

	//open file and error check that it exists
	int fd = open(dictionaryName, O_RDONLY, 0);
	if(fd<0){
		printf("File %d not found.\n",fd);
		exit(1);
	}

	//initialize buffer and attempt
	char *buffer = (char *)malloc(length);
	int count=0;
	int padding = (sizeof(buffer)-strlen(word));
	for(int i=0;i<padding;i++){
		strcat(word," ");
	}
	int attempt = sizeof(buffer);
	int gotWord;
	//main while loop to read in [attempt] bytes at a time
	while((gotWord=read(fd,buffer,attempt)) > 0){	
		count++;		
		if(strcmp(buffer,word) == 0){
			printf("Word matched at line <%d>.\n",count);
			fflush(stdout);
			exit(0);
		}	
		write(1,buffer,gotWord);	
	}
	printf("Word not found, reached line <-%d>.\n",count);
	free(buffer);
	close(fd);
	return 0;
}

int main(int argc, char *argv[]){
	//mostly a test case to use cmd line arguments
	char *dict=argv[1];
	char *word=argv[2];
	int length=atoi(argv[3]);
	lineNum(dict,word,length);
}
