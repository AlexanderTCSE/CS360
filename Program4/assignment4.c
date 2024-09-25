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

void execCmd(char *cmd);

int main(int argc, char *argv[]){
	//handle memory for input string	
	size_t length=0;
	for(int i=1;i<argc;i++){
		length += strlen(argv[i])+1;
	}
	char *input=malloc(length);

	//strcat all args into one input string
	input[0]='\0';
	for(int i=1;i<argc;i++){
		strcat(input,argv[i]);
		//put spaces between args until the last arg
		if(i < argc-1){
			strcat(input," ");
		}
	}
	//printf("Full input: <%s>\n",input);

	//split full input into two strings cmd1 and cmd2 for exec-ing	
	char *cmd1 = strtok(input,":");
	char *cmd2 = strtok(NULL,":");	
	//printf("cmd 1: <%s>\n",cmd1);
	//printf("cmd 2: <%s>\n",cmd2);

	int pipefd[2];
	pipe(pipefd);
	pid_t pid = fork();
	//error fork
	if(pid == -1){
		fprintf(stderr, "%s\n",strerror(errno));
		free(input);
		return EXIT_FAILURE;
	}
	//child fork
	if(pid == 0){
		if(cmd2 != NULL){
			close(pipefd[1]);		//close write end of child
			dup2(pipefd[0], STDIN_FILENO);	//redirect stdin to read end of pipe
			close(pipefd[0]);		//close original read end of pipe
			execCmd(cmd2);			//execute command	
		}else{
			exit(0);	//if no cmd2, just exit out
		}	
	//parent fork
	}else{	
		if(cmd2 != NULL){
			//if cmd2, pipe		
			close(pipefd[0]);		//close read end of parent
			dup2(pipefd[1], STDOUT_FILENO);	//redirect stdout to write end of pipe
			close(pipefd[1]);		//close write end of pipe		
		}else{	
			//if no cmd2, no pipe
			close(pipefd[0]);
			close(pipefd[1]);
		}
		execCmd(cmd1);
	}
	int status;
	wait(&status);

	free(input);	
	return 0;
}

void execCmd(char *cmd){
	char *args[64];
	int i=0;
	char *token=strtok(cmd," ");	
	while(token != NULL){
		args[i++]=token;
		token=strtok(NULL," ");
	}
	args[i]=NULL;
	execvp(args[0], args);
}
