#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Item.h"
#include "ST.h"

#define lgNmax 30

/* skip list */

typedef struct STnode *link;
struct STnode{
	
	Item item; 
	int qnt;
	
	link *next;
	link prox;
	int sz; 
};

static link head;
static int N, lgN;

link NEW(Item item, int k)      
{
	int i; 
	link x = malloc(sizeof *x);
	x->next = malloc(k*sizeof(link));
	x->item = item; x->sz = k; x-> prox = NULL;
	
	if(item == NULLitem) x-> qnt = 999999;
	else x-> qnt = 1;
	
	for (i = 0; i < k; i++) x->next[i] = NULL;
	return x;                         
}                                   

void STinit(int max) 
{ 
	N = 0; lgN = 0; 
	head = NEW(NULLitem, lgNmax); 
	printf("Inicializou estrutura: Skip List.\n\n");
}

void deleteR(link t, Item v, int k){
	if (t->next[k] == NULL) {
		if (k > 0) {
			deleteR(t, v, k-1);
			return;
		}
	} else if ( eq(v, t->next[k]->item) ) {
		t->next[k] = t->next[k]->next[k];
		if(k > 0) deleteR(t, v, k-1);
	
		return;
	} else if (less(v, t->next[k]->item) ){
		if (k > 0)
			deleteR(t, v, k-1);
	}
	else deleteR(t->next[k], v, k);
}

void STdelete(Key v)
{
	deleteR(head, v, lgN); N--; 
}

Item searchR(link t, Key v, int k, int add)
{
	if (t->next[k] == NULL) { 
		if (k == 0) return NULLitem;{
			return searchR(t, v, k-1,add);
		}			
	}
	if (eq(v, key(t->next[k]->item))){
		t->next[k]->qnt += add;
		if(t->next[k]->qnt == 0){
			return NULLitem;
		}
		return t->next[k]->item;
	}
	if (less(v, key(t->next[k]->item))){ 
		if (k == 0) return NULLitem;{
			return searchR(t, v, k-1,add); 
		}
	}
	return searchR(t->next[k], v, k, add);
}

Item STsearch(Key v, int add)
{
	return searchR(head, v, lgN, add); 
}

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

link merge(link a, link b) {
    struct STnode h;
    link c = &h;
    while (a != NULL && b != NULL)
		/* c é o último nó da lista cuja cabeça é h */
		/*if (less(a->item, b->item)) {*/
		if (a -> qnt >= b -> qnt) {
			c->prox = a;
			c = a; a = a->prox;
		}
		else {
			c->prox = b;
			c = b; b = b->prox;
		}
    c->prox = (a == NULL) ? b : a;
    return h.prox;
}

link mergesort(link c) {
    link a, b;
    if (c == NULL || c->prox == NULL) return c;
    a = c;
    b = c->prox;
    while (b != NULL && b->prox != NULL) {
        c = c->prox;
        b = b->prox->prox;
    }
    b = c->prox;
    c->prox = NULL;
    return merge(mergesort(a), mergesort(b));
}

void STsort(void (*visit)(Item, int), int n)
{
	link t = head;
	
	while (t->next[0] != NULL) {
		t -> prox = t -> next[0];
		t = t->next[0];
		/*visit(t->item, t->qnt);*/
	}
	t -> prox = NULL;
	t = head;
	mergesort(t);

	while(t->prox != NULL){
		t = t->prox;
		visit(t->item, t->qnt);
		if(--n == 0) break;
	}
}

void freee()
{
	link del = NULL, list;
	list = head;
	while(list != NULL){
		free(del);
		del = list;
		list = list -> prox;
	}
	free(del);
	return;
}

