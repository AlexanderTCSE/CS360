//Alexander Tattersall
//CS360 - Fall Term
//pairsofwords, main file
//will handle file I/O and related error checking
//mostly makes calls to hash.c to do the hard work

#include "include.h"

int isNum(char *str);

int main(int argc, char *argv[]){
	//variable initialization
	int start;		//index of argv to start reading files at
	int numPairs=-1;	//number of pairs to print. Init to -1 as a flag that numPairs is unchanged
	struct HASHENTRY *ht;	//hashtable struct
	FILE *fp;		//file being read
	char *word1=NULL;	//first word of pair
	char *word2=NULL;	//second word of pair
	char *wordpair=NULL;	//concatenated pair

	if(argc<2){	//if cmd line has less than one argument, report error
		printf("Usage: pairsofwords <-count> <file1> <file2> ... <fileN>\n");
		return 1;
	}

	//convert number flag to an int
	if(argv[1][0] == '-' && isNum(argv[1]+1)){
		numPairs=atoi(argv[1]+1);	//convert argument to int
		if(numPairs < 0){	//if negative, invert
			numPairs=-numPairs;	//abs value
		}	
		if(numPairs==0){
			printf("Error: cannot print 0 pairs.\n");
			return 1;
		}	
		start=2;
		if(argc<3){
			printf("Error: no file names specified.\n");
			return 1;
		}
	}else{
		start=1;
		numPairs=0;
		if(argc<2){
			printf("Error: no file names specified.\n");
			return 1;
		}
	}
	
	//initialize hash table
	ht=inithash(HASHSIZE);
	if(ht==NULL){
		fprintf(stderr,"Error initializing hash table.\n");
		return 1;
	}
	
	//main loop to read through files
	for(int i=start;i<argc;i++){		//loop through argv, checking files
		FILE *fp = fopen(argv[i], "r");	//open current cmd line arg if it is a file
		if(fp==NULL){			//if file is null, print an error and quit
			printf("Error opening file <%s>, exiting.\n", argv[i]);
			exit(1);
		}
		//printf("Reading file: %s\n", argv[i]); debug line
		
		//try to get next word, if NULL close file and continue to next
		word1=getNextWord(fp);	
		if(word1==NULL){
			fclose(fp);
			continue;
		}
		//if word1 succeeds, get word2
		word2=getNextWord(fp);
		//while words are available, read and make pairs
		while(word1 != NULL && word2 != NULL){
			if(word2==NULL){
				break;
			}

			size_t len = strlen(word1)+strlen(word2)+2;	//get length of word pair + space + nullterm
			wordpair=(char*) malloc(len * sizeof(char));	//allocate memory as len*char
			if(wordpair==NULL){				//error checking
				printf("Could not allocate memory for word pair.\n");
				free(word1);
				free(word2);
				fclose(fp);
				exit(1);
			}
			
			strcpy(wordpair,word1);	//copy word1 into pair
			strcat(wordpair," ");	//add a space
			strcat(wordpair,word2);	//add second word	
			//printf("word pair generated: <%s>\n",wordpair); debug line

			addhash(ht,wordpair);	//add pair to hash table
			free(wordpair);		//free pair memory
			free(word1);		//free word1 memory
			word1=word2;		//set word1 to word2
			word2=getNextWord(fp);	//set word2 to next word from file
		}	
		//free word memory after reading file
		free(word1);
		free(word2);
		fclose(fp);			//close file when done
	}
	//after reading files, print # of entries
	printhash(ht,numPairs);
	//free after output
	freehash(ht);	
	return 0;
}

//function to check if first cmd line argument is a number of pairs to print
int isNum(char *str){
	if(*str=='-'){
		str++;
	}
	while(*str){
		if(!isdigit(*str)){
			return 0;
		}
		str++;
	}
	return 1;
}
