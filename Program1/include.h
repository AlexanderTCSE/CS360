//Alexander Tattersall
//Include file to include everything else
//Also includes definitions, typedefs for nodes, etc

#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<ctype.h>
#include<stdlib.h>

#define HASHSIZE 100

//Table nodes typedef
typedef struct node
{
	void *pairs;
	int count;
	struct node *next;
}node;

//Hash table typedef
typedef struct HASHENTRY
{
	struct node **table;
	size_t size;
	size_t count;
}HASHENTRY;

//function prototypes for hashing
void freelist(struct node *sent);
int hash(char *pairs, size_t table_size);
struct HASHENTRY *inithash(size_t table);
void addhash(struct HASHENTRY *hashtable, char *pairs);
void freehash(struct HASHENTRY *hashtable);
void printhash(struct HASHENTRY *hashtable, int num);
void resizehash(struct HASHENTRY *hashtable);
struct node *newNode(char *pairs);
int compare(const void *a, const void *b);

//getWord include block, from getWord.h
#ifndef GETWORD_H
#define GETWORD_H

#include <stdio.h>

#define DICT_MAX_WORD_LEN	256		/* maximum length of a word (+1) */

/* Reads characters from fd until a single word is assembled */
/* and returns a copy of the word allocated from the heap.   */
/* NULL is returned at EOF.                                  */

/* Words are defined to be separated by whitespace and start */
/* with an alphabetic character.  All alphabetic characters  */
/* translated to lower case and punctuation is removed.      */

char* getNextWord(FILE* fd);

#endif

//crc64 hash function include block, from crc64.h
#ifndef CRC64_H
#define CRC64_H

/* interface to CRC 64 module */

/* crc64 takes a string argument and computes a 64-bit hash based on */
/* cyclic redundancy code computation.                               */

unsigned long long crc64(char* string);

#endif
