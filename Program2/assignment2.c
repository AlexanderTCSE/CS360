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

//homemade string prototypes
int strlength(const char *string);
void copy(char *word1, char *word2);

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
	int filesize=lseek(fd,0,SEEK_END);
	if(filesize==-1){
		write(2,strerror(errno),strlength(strerror(errno)));
		close(fd);
		return errno;
	}

	//initialize search range based on file size
	int numLines=filesize/length;

	//initialize buffer to read in fixed amt of bytes
	char buffer[length+1];
	char inWord[length+1];

	copy(word,inWord);
	inWord[length]='\0';

	//if user's word is longer than length, truncate
	if(strlength(word) >= length){
		inWord[length-1]='\n';
		inWord[length]='\0';
	}

	//pad user's word to match the bytes in buffer	
	int padding = length-strlength(word)-1;
	for(int i=0;i<padding;i++){
		strcat(inWord," ");	//add whitespace to pad
	}
	inWord[length-1]='\n';		//length-1 is a newline
	inWord[length]='\0';		//finish with nullterm to make it a string
	lseek(fd,0,SEEK_SET);		//reset seek head to beginning of fd
	
	//other initializations		
	int gotWord;
	int low=0;
	int high=numLines-1;
	int mid;	

	//main while loop to read in [length] bytes at a time, and conduct binary search	
	while(low <= high){	
		mid=low+(high-low)/2;
		int offset=mid*length;

		//seek to middle of file, begin reading
		lseek(fd,offset,SEEK_SET);
		int gotWord=read(fd,buffer,length);
		buffer[gotWord]='\0';
		int cmp=strcmp(buffer,inWord);

		//if 0, word matched, else do binary search
		if(cmp==0){
			printf("Word matched at line <%d>.\n",mid+1);
			close(fd);
			return mid+1;
		}else if(cmp<0){
			low=mid+1;
		}else{
			high=mid-1;
		}
	}

	//edge case handling
	if(low==0){
		printf("Word not found, reached line <-1>.\n");
		close(fd);
		return -1;
	}
	printf("Word not found, reached line <-%d>.\n",low);	
	close(fd);
	return -low;
}

//homemade strlen function
int strlength(const char *string){
	int len=0;	
	while(string[len] != '\0'){
		len++;
	}
	return len;
}

//homemade strcpy function
void copy(char *word1, char *word2){
	while(*word1 != '\0'){
		*word2 = *word1;
		word1++;
		word2++;
	}
	*word2='\0';
}


int main(int argc, char *argv[]){
	//mostly a test case to use cmd line arguments
	char *dict=argv[1];
	char *word=argv[2];
	int length=atoi(argv[3]);
	lineNum(dict,word,length);
}

