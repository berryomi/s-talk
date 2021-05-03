#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "List.h"

#define LIST_MAX 10
#define NODE_MAX 30

LIST listHead[LIST_MAX]; // list heads
node_t listNode[NODE_MAX]; // list nodes

int used_lists = 0;
int free_lists = LIST_MAX; // 10
int used_nodes = 0;
int free_nodes = NODE_MAX; // 30

LIST * ListCreate(){
	LIST * new = NULL;
	if (used_lists < LIST_MAX){
		new = &listHead[used_lists]; 
		new->head = NULL;
		new->tail = NULL;
		new->curr = NULL;
		used_lists++;
		free_lists--;
	}
	else if (free_lists == 0){
		return NULL;
	}
	return new;
}

int ListCount(LIST * L){
	node_t * curr = L->head;
	unsigned int count = 0;
	if (L == NULL){
		count = -1;
	}
	while (curr != NULL){
		count++;
		curr = curr->next;
	}
	return count;
}

void *ListFirst(LIST * L){
	node_t * curr = L->head;

	if (L == NULL || L->head == NULL){
		return NULL;
	}
	else{
		L->curr = curr;
		return curr; 
	}
}

void *ListLast(LIST * L){
	node_t * curr = L->tail;
	
	if (L == NULL || L->head == NULL){
		return NULL;
	}
	else{
		L->curr = L->tail;
		return curr->data; 
	}
}

void *ListNext(LIST * L){
	node_t * curr = L->curr;

	if (L == NULL || L->head == NULL){
		return NULL;
	}
	else if (curr == L->tail){ // curr is tail = no next node
		return NULL;
	}
	else{ // general case
		curr = curr->next;
		L->curr = curr;
		return curr; 
	}
}

void *ListPrev(LIST * L){
	node_t * curr = L->curr;

	if (L == NULL || L->head == NULL){
		return NULL;
	}
	else if (curr == L->head){ // curr is head = no prev node
		return NULL;
	}
	else{ // general case
		curr = curr->prev;
		L->curr = curr;
		return curr; 
	}
}

void *ListCurr(LIST * L){
	node_t * curr = L->curr;

	if (L == NULL || L->head == NULL){
		return NULL;
	}
	else{
		return curr; 
	}
}

int ListPrepend(LIST * L, void * item){
	if (L == NULL){
		return -1;
	}
	if (used_nodes < NODE_MAX){ // check for free spaces in the node list
		used_nodes++;
		free_nodes--;
		listNode[used_nodes].data = item;
		listNode[used_nodes].next = L->head;
		listNode[used_nodes].prev = NULL;
		
		node_t * temp = L->head;
		if (L->head == NULL){ // if empty list
			L->tail = &listNode[used_nodes]; // then head == tail
		}
		else{
			temp->prev = &listNode[used_nodes];
		}
		L->head = &listNode[used_nodes]; // new head
		L->curr = L->head;

		return 0;
	}
	return -1;
}

int ListAppend(LIST * L, void * item){
	if (L == NULL){
		return -1;
	}
	if (used_nodes < NODE_MAX){ // check for free spaces in the node list
		used_nodes++;
		free_nodes--;
		listNode[used_nodes].data = item;
		listNode[used_nodes].prev = L->tail;
		listNode[used_nodes].next = NULL;
		
		node_t * temp = L->tail;
		if (L->head == NULL){ // empty list
			L->head = &listNode[used_nodes]; // then head == tail
		}
		else{ // there is at least one node
			temp->next = &listNode[used_nodes]; 
		}
		L->tail = &listNode[used_nodes]; // new tail
		L->curr = L->tail;

		return 0;
	}
	return -1;
}


int ListAdd(LIST * L, void * item){
	if (L == NULL){ // if no list exists
		return -1;
	}
	if (used_nodes < NODE_MAX){ // if list exists and there is enough space
		node_t * tempHead = L->head;
		node_t * tempTail = L->tail;

		used_nodes++;
		free_nodes--;
		listNode[used_nodes].data = item;

		if (L->head == NULL){ // if empty list
			listNode[used_nodes].prev = NULL;
			listNode[used_nodes].next = NULL;
			L->head = &listNode[used_nodes];
			L->tail = &listNode[used_nodes];
			L->curr = L->head; // now there is only one element in the list
		}
		else if (L->head == L->tail){ // if only one element
			// append
			listNode[used_nodes].prev = L->tail;
			listNode[used_nodes].next = NULL;
		
			tempTail->next = &listNode[used_nodes];
			L->tail = &listNode[used_nodes]; 
			L->curr = L->tail;
		}
		else if (L->curr == L->head->prev){ // if current pointer is before head
			//prepend
			listNode[used_nodes].prev = NULL;
			listNode[used_nodes].next = L->head;
			
			tempHead->prev = &listNode[used_nodes];
			L->head = &listNode[used_nodes];
			L->curr = L->head;
		}
		else if(L->curr == L->tail->next || L->curr == L->tail){ // if current pointer is tail 
			//append
			listNode[used_nodes].prev = L->tail;
			listNode[used_nodes].next = NULL;
		
			tempTail->next = &listNode[used_nodes];
			L->tail = &listNode[used_nodes]; 
			L->curr = L->tail;
		}
		else{ // general case
			node_t * tempCurr = L->curr;
			node_t * tempNext = L->curr->next;

			listNode[used_nodes].prev = tempCurr;
			listNode[used_nodes].next = tempNext;
			tempCurr->next = &listNode[used_nodes];
			tempNext->prev = &listNode[used_nodes];
			L->head = tempHead;
			L->tail = tempTail;
			L->curr = &listNode[used_nodes];
		}
		return 0;
	}
	return -1;
}


int ListInsert(LIST * L, void * item){
	if (L == NULL){ // if no list exists 
		return -1;
	}
	if (used_nodes < NODE_MAX){ // if list exists and there is enough space
		node_t * tempHead = L->head;
		node_t * tempTail = L->tail;

		used_nodes++;
		free_nodes--;
		listNode[used_nodes].data = item;

		if (L->head == NULL){ // if empty list
			listNode[used_nodes].prev = NULL;
			listNode[used_nodes].next = NULL;
			L->head = &listNode[used_nodes];
			L->tail = &listNode[used_nodes];
			L->curr = L->head; // now there is only one element in the list
		}
		else if (L->head == L->tail){ // if only one element
			//prepend
			listNode[used_nodes].prev = NULL;
			listNode[used_nodes].next = L->head;
			
			tempHead->prev = &listNode[used_nodes];
			L->head = &listNode[used_nodes];
			L->curr = L->head;
		}
		else if (L->curr == L->head->prev || L->curr == L->head){ // if current pointer is (before) head
			//prepend
			listNode[used_nodes].prev = NULL;
			listNode[used_nodes].next = L->head;
			
			tempHead->prev = &listNode[used_nodes];
			L->head = &listNode[used_nodes];
			L->curr = L->head;
		}
		else if(L->curr == L->tail->next){ // if current pointer is after tail 
			//append
			listNode[used_nodes].prev = L->tail;
			listNode[used_nodes].next = NULL;
		
			tempTail->next = &listNode[used_nodes];
			L->tail = &listNode[used_nodes]; 
			L->curr = L->tail;
		}
		else{ // general case
			node_t * tempCurr = L->curr;
			node_t * tempPrev = L->curr->prev;

			listNode[used_nodes].prev = tempPrev;
			listNode[used_nodes].next = tempCurr;
			tempCurr->prev = &listNode[used_nodes];
			tempPrev->next = &listNode[used_nodes];
			L->head = tempHead;
			L->tail = tempTail;
			L->curr = &listNode[used_nodes];
		}
		return 0;
	}
	return -1;

}


void *ListRemove(LIST *L){
	if (L == NULL){
		return NULL;
	}
	if(L->head == NULL){ // empty list
      	return NULL;
   	}
   	if (L->curr == L->head->prev || L->curr == L->tail->next){ // if current points to null, do nothing
		// remove nothing because curr is not pointing to any value
		return NULL;
	}

	node_t * remove = L->curr; // Remove this node
	node_t * tempPrev = L->curr->prev;
	node_t * tempNext = L->curr->next;

   	if (L->head == L->tail){ // if only one element
   		L->head = NULL;
   		L->tail = NULL;
   		L->curr = NULL;
   	}
   	else if (L->curr == L->head){ // if removing head (head change)
   		L->head = L->head->next;
   		L->head->prev = NULL;
   		L->curr = L->head;
   	}
   	else if (L->curr == L->tail){ // if removing tail (tail change)
   		L->tail = L->tail->prev;
   		L->tail->next = NULL;
   		L->curr = L->tail;
   	}
	else{ // general case
		tempPrev->next = L->curr->next;
		tempNext->prev = L->curr->prev;
		
		L->curr = tempNext;
	}
	remove->next = NULL;
	remove->prev = NULL;
	remove->data = NULL;
	used_nodes--; // re-allocate spaces
	free_nodes++;
	return remove->data;

}


 void *ListTrim(LIST * L){
   if (L == NULL){
      return NULL;
   }
   L->curr = L->tail; // point to the last item
   return ListRemove(L);
}

void itemFree(){
	used_nodes--; // re-allocate spaces
	free_nodes++;
	
	listNode[free_nodes].next = NULL;
	listNode[free_nodes].prev = NULL;
	listNode[free_nodes].data = NULL;
}

void ListFree(LIST * L, void (*itemFree)(void *)){
	if (L == NULL || L->head == NULL){
		return;
	}
	node_t * curr = L->head;
	while (L->curr != NULL){
		curr = L->curr;
		L->curr = L->curr->next;
		(*itemFree)(curr);
	}
	used_lists--;
	free_lists++;
	
	L->head = NULL;
	L->tail = NULL;
	L->curr = NULL;
}


void ListConcat (LIST * L1, LIST * L2){
	if (L1 == NULL || L2 == NULL){
		return;
	}
	if (L2->head == NULL){ // if L2 is empty, just leave L1 is at it is
		return;
	}
	if (L1->head == NULL){ // if L1 is empty
		L1->head = L2->head;
		L1->tail = L2->tail;
		L1->curr = L2->curr;

		ListFree(L2, itemFree);
		return;
	}
	// general case (both L1 and L2 not empty)
	L1->tail->next = L2->head;
	L2->head->prev = L1->tail;
	L1->tail = L2->tail;
	ListFree(L2, itemFree);
}

int comparator (const void *a, const void *b){
	if (a == b){
        return 1;
	}
    return 0;
}

void *ListSearch(LIST * L, int (* comparator)(void *a, void *b), void * comparisonArg){
	if (L == NULL){
		return NULL;
	}
	else if (L->head == NULL){
		return NULL;
	}
	node_t * curr = L->curr;
	while (curr != NULL){
		if ((*comparator)(curr->data, comparisonArg)){ 
			L->curr = curr;
			return curr->data;
		}
		curr = curr->next;
	}
	return NULL;
}

void LLprint(LIST * L) {
	if (L == NULL || L->head == NULL){
		return;
	}
	if (used_nodes <= NODE_MAX){
		for (node_t * current = L->head; current != NULL ; current = current->next) {
			printf("%d ", current->data);
	}
	putchar('\n');
	}
}

void * asData(node_t * value){
	if (value == NULL){
		return NULL;
	}
	else{
		return value->data;
	}
}