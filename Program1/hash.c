//Alexander Tattersall
//CS360 Fall Term
//Professor Ben McCamish
//Hash functions for initializing, adding to, resizing hash table of word pairs

#include"include.h"

//init functionn, declares an array of pointers to hashentries, malloc'd memory
struct HASHENTRY *inithash(size_t table_size){
	struct HASHENTRY *hashtable=malloc(sizeof(struct HASHENTRY));
	if(hashtable==NULL){
		fprintf(stderr,"Error allocating hash table memory\n");
		return NULL;
	}
	hashtable->table=calloc(table_size, sizeof(struct node*));
	if(hashtable->table==NULL){
		fprintf(stderr,"Error allocaitng memory for hash table array\n");
		free(hashtable);
		return NULL;
	}
	hashtable->size=table_size;
	hashtable->count=0;
	return hashtable;
}

//hash function, mostly just calls crc64
int hash(char *pairs, size_t table_size){
	unsigned long long hash_value=crc64(pairs);
	return(int)(hash_value % table_size);
}

//function to make a new node of pair, count, next pointer
struct node* newNode(char *pairs){
	struct node *new_node=malloc(sizeof(struct node));
	new_node->pairs=strdup(pairs);
	new_node->count=1;
	new_node->next=NULL;
	return new_node;
}

//function to add a word pair to the hash table
void addhash(struct HASHENTRY *hashtable, char *pairs){
	int index=hash(pairs, hashtable->size);
	struct node *current=hashtable->table[index];	

	//search loop to see if word pair already exists, increment count if it does
	while(current != NULL){
		if(strcmp(current->pairs, pairs)==0){	//compare current node pair to scrolled pair from file
			current->count++;	//increment pair if a match is found
			return;
		}
		current=current->next;
	}

	//if we get here, pair is not found, add a new node
	struct node *new_node=newNode(pairs);
	new_node->next=hashtable->table[index];
	hashtable->table[index]=new_node;
	hashtable->count++;

	//check if table needs to be resized (75% load)
	if(hashtable->count > hashtable->size * 0.75){
		resizehash(hashtable);
	}
}

//function to resize hash table and rehash all elements
void resizehash(struct HASHENTRY *hashtable){
	size_t new_size=hashtable->size*3;
	//struct HASHENTRY *new_hashtable=inithash(new_size);
	struct node **new_table=calloc(new_size,sizeof(struct node*));

	if(new_table==NULL){
		fprintf(stderr,"Failed to alloc memory for new hash table\n");
		return;
	}

	//rehash everything
	size_t i;
	for(size_t i=0;i<hashtable->size;i++){
		struct node *current=hashtable->table[i];
		while(current != NULL){
			//addhash(new_hashtable,current->pairs);			
			struct node *next=current->next;
			int new_index=hash(current->pairs, new_size);
			current->next=new_table[new_index];
			new_table[new_index]=current;
			current=next;
		}
		//freelist(hashtable->table[i]);
	}
	free(hashtable->table);
	hashtable->table=new_table;
	hashtable->size=new_size;
}

//function to free a list of node
void freelist(struct node *sent){
	struct node *current=sent;
	while(current != NULL){
		struct node *next = current->next;
		free(current->pairs);
		free(current);
		current=next;
	}
}

//function to free a hash table
void freehash(struct HASHENTRY *hashtable){
	for(size_t i=0;i<hashtable->size;i++){
		freelist(hashtable->table[i]);
	}
	free(hashtable->table);
	free(hashtable);
}

//function to sort and print the hash table as an array
void printhash(struct HASHENTRY *hashtable, int num){
	struct node **entries=malloc(hashtable->count * sizeof(struct node*));
	size_t idx=0;

	for(size_t i=0;i<hashtable->size;i++){
		struct node *current=hashtable->table[i];
		while(current != NULL){
			entries[idx++]=current;
			current=current->next;
		}
	}

	qsort(entries, hashtable->count, sizeof(struct node*), compare);

	size_t maxEntries;
	if(num==-1){
		maxEntries=hashtable->count;
	}else{
		maxEntries=num;
	}

	for(size_t i=0; i<hashtable->count && i<maxEntries;i++){
		printf("%10d %s\n", entries[i]->count, (char*)entries[i]->pairs);
	}

	free(entries);
}

//comparator for qsort
int compare(const void *a, const void *b){
	struct node *entryA = *(struct node**)a;
	struct node *entryB = *(struct node**)b;
	return entryB->count - entryA->count;
}
