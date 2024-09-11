/***********************************************************************
name: Alexander Tattersall
	lineNum -- see if a word is in the online dictionary
description:	
	See CS 360 IO lecture for details.
***********************************************************************/

#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

//homemade strlen prototype
int strlength(const char *string);

/**********************************************************************
Search for given word in dictionary using file descriptor fd.
Return the line number the word was found on, negative of the last line searched
if not found, or the error number if an error occurs.
**********************************************************************/
int lineNum(char *dictionaryName, char *word, int length) {

	//open file and error check that it exists
	int fd = open(dictionaryName, O_RDONLY, 0);
	if(fd==-1){
		write(2,strerror(errno),strlength(strerror(errno)));
		return errno;
	}

	//find number of line in file
	off_t filesize=lseek(fd,0,SEEK_END);
	if(filesize==-1){
		write(2,strerror(errno),strlength(strerror(errno)));
		close(fd);
		return errno;
	}

	//initialize search range based on file size
	int numLines=filesize/length;

	//initialize buffer to read in fixed amt of bytes
	char buffer[length];

	//if user's word is longer than length, truncate
	if(strlength(word) >= length){
		word[length-1]='\n';
		word[length]='\0';
	}

	//pad user's word to match the bytes in buffer	
	int padding = length-strlength(word)-1;
	for(int i=0;i<padding;i++){
		strcat(word," ");	//add whitespace to pad
	}
	word[length-1]='\n';		//length-1 is a newline
	word[length]='\0';		//finish with nullterm to make it a string
	lseek(fd,0,SEEK_SET);		//reset seek head to beginning of fd
	
	//other initializations		
	int gotWord;
	int low=0;
	int high=numLines=1;
	int mid;
	int count=0;

	//main while loop to read in [length] bytes at a time	
	while((gotWord=read(fd,buffer,length)) > 0){	
		count++;		
		buffer[gotWord]='\0';
		if(strcmp(buffer,word) == 0){
			printf("Word matched at line <%d>.\n",count);
			fflush(stdout);	
			close(fd);
			return count;
		}			
	}
	printf("Word not found, reached line <-%d>.\n",count);	
	close(fd);
	return -count;
}

//homemade strlen function
int strlength(const char *string){
	int len=0;	
	while(string[len] != '\0'){
		len++;
	}
	return len;
}

int main(int argc, char *argv[]){
	//mostly a test case to use cmd line arguments
	char *dict=argv[1];
	char *word=argv[2];
	int length=atoi(argv[3]);
	lineNum(dict,word,length);
}
