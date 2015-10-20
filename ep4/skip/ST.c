/*
 * This code is from "Algorithms in C, Third Edition,"
 * by Robert Sedgewick, Addison Wesley Longman, 1998.
 * Possibly with some minor editing.  Check the book!
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Item.h"
#include "ST.h"

#define lgNmax 30

typedef struct STnode *link;
struct STnode{
	Item item; 
	link *next;
	int sz; 
};

static link head;
static int N, lgN;

link NEW(Item item, int k)      
{
	int i; 
	link x = malloc(sizeof *x);
	x->next = malloc(k*sizeof(link));
	x->item = item; x->sz = k;
	for (i = 0; i < k; i++) x->next[i] = NULL;
	return x;                         
}                                   

void STinit(int max) 
{ 
	N = 0; lgN = 0; 
	head = NEW(NULLitem, lgNmax); 
}

/* prog3.7.c */

Item searchR(link t, Key v, int k)
{
	if (t->next[k] == NULL) { 
		if (k == 0) return NULLitem;{
			return searchR(t, v, k-1);
		}			
	}
	if (eq(v, key(t->next[k]->item))){
		return t->next[k]->item;
	}
	if (less(v, key(t->next[k]->item))){ 
		if (k == 0) return NULLitem;{
			return searchR(t, v, k-1); 
		}
	}
	return searchR(t->next[k], v, k);
}

Item STsearch(Key v)
{
	return searchR(head, v, lgN); 
}

/* prog13.9.c */

int randX()
{
	int i, j, t = rand();
	for (i = 1, j = 2; i < lgNmax; i++, j += j)
		if (t > RAND_MAX/j) break;
	if (i > lgN) lgN = i;
	return i;
}

void insertR(link t, link x, int k)
{
	Key v = key(x->item); 
    if ((t->next[k] == NULL) || less(v, key(t->next[k]->item))) 
    { 
        if (k < x->sz)
        {
			x->next[k] = t->next[k]; 
			t->next[k] = x; 
		}
        if (k == 0) return;
        insertR(t, x, k-1); return;
    }
	insertR(t->next[k], x, k);
}

void STinsert(Item item)
{	
	insertR(head, NEW(item, randX()), lgN); 
	N++; 
}

/* prog13.10.c */

void deleteR(link t, Key v, int k)
{
	if (t->next[k] == NULL) {
		if (k > 0) {
			deleteR(t, v, k-1);
			return;
		}
	}
	else if (eq(v, key(t->next[k]->item))){
		t->next[k] = t->next[k]->next[k];
		deleteR(t, v, k-1);
		return;
	}
	else if (less(v, key(t->next[k]->item))){ 
		if (k > 0) deleteR(t, v, k-1);
	}
	else deleteR(t->next[k], v, k);
}

void STdelete(Key v)
{
	deleteR(head, v, lgN); N--; 
}

/* STsort() and STcount() not in the book */

void STsort(void (*visit)(Item))
{
	link t = head;
	while (t->next[0] != NULL){
		t = t->next[0];
		visit(t->item);
	}
}

int STcount(){
	return N; 
}

