#include <stdlib.h>
#include <stdio.h>

typedef struct _node {
	void * data;
	struct _node * prev;
	struct _node * next;
} node_t;

typedef struct {
	node_t * head;
	node_t * tail;
	node_t * curr;
} LIST;

//prototypes
LIST *ListCreate();
int ListCount(LIST * L);
void *ListFirst(LIST * L);
void *ListLast(LIST * L);
void *ListNext(LIST * L);
void *ListPrev(LIST * L);
void *ListCurr(LIST * L);
int ListAdd(LIST *list, void *item);
int ListInsert(LIST * L, void * item);
int ListAppend(LIST * L, void * item);
int ListPrepend(LIST * L, void * item);
void *ListRemove(LIST * L);
void ListConcat(LIST * L1, LIST * L2);
void ListFree(LIST *L, void (*itemFree)(void *));
void *ListTrim(LIST *L);
void *ListSearch(LIST * L, int (* comparator)(void *a, void *b), void * comparisonArg);

// helper functions
void LLprint(LIST * L);
int comparator (const void *a, const void *b);
void itemFree();
