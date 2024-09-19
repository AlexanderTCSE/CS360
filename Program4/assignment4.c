/***********************************************************************
name:
	assignment4 -- acts as a pipe using ":" to seperate programs.
description:	
	See CS 360 Processes and Exec/Pipes lecture for helpful tips.
***********************************************************************/

/* Includes and definitions */
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>

/**********************************************************************
./assignment4 <arg1> : <arg2>

    Where: <arg1> and <arg2> are optional parameters that specify the programs
    to be run. If <arg1> is specified but <arg2> is not, then <arg1> should be
    run as though there was not a colon. Same for if <arg2> is specified but
    <arg1> is not.
**********************************************************************/


int main(int argc, char *argv[]){
	//handle memory for input string	
	size_t length=0;
	for(int i=1;i<argc;i++){
		length += strlen(argv[i])+1;
	}
	char *input=malloc(length);

	//strcat all args into one input string
	strcpy(input,argv[1]);	
	for(int i=2;i<argc;i++){
		strcat(input," ");
		strcat(input,argv[i]);
	}
	printf("Full input: <%s>\n",input);

	//split full input into two strings cmd1 and cmd2 for exec-ing
	//also remove trailing and leading spaces from cmd1 and cmd2
	char *cmd1 = strtok(input,":");
	//this is sort of ugly. if something breaks with input, it's probably here
	cmd1[strlen(cmd1)-1]=0;
	char *cmd2 = strtok(NULL,": ");
	printf("cmd 1: <%s>\n",cmd1);
	printf("cmd 2: <%s>\n",cmd2);

	//[TODO] the rest
	//figure out fork(), and create two processes on either end of a pipe()
	//cmd1 should be parent and run with stdout=pipe write end
	//cmd2 should be child and run with stdin=pipe read end
	//LOOK AT PIPES.C ON CANVAS

	free(input);	
	return 0;
}
