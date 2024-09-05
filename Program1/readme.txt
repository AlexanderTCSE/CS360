/*======================================*\
|| Alexander Tattersall		 	||
|| CS360 - Systems Programming		||
|| Assignment 1 - Word Pair Detector 	||
\*======================================*/
Usage: pairsofwords <-count> fileName1 <fileName2> ... <fileNameN>

To Compile and Run:
1	tar xvf pairsofwords.tar
2	make
3	./pairsofwords <-count> fileName1 <fileName2> ... <fileNameN>

Program to take in N number of text files, detect occurences of pairs of words, store those occurences in a chained hash table
and print a specified (by -count) number of pairs to stdout. If no count is specified, print the entire contents of the hash
table to stdout.

main.c:
main.c contains driver code for the program, handling file I/O, related error checking, and generation
of word pairs. Storage of word pairs and counting is done via calls to hash.c, which contains all the
functions for hashing.

hash.c:
hash.c contains functions to initialize a hash table pointer structure, add nodes to that table, hash
word pairs, print sorted contents, and resize the hash table once a certain load limit 
(75% in this case) is reached.
Also contains functions to free memory associated with hash table and linked lists.

include.h:
instead of separate .h files, I created one include.h file for all function prototypes. The provided
crc64.h and getWord.h files are included here as well.
