#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Item.h"
#include "ST.h"

typedef struct STnode* link;

struct STnode{

	Item item; 
	int qnt;
	
	link l, r; 
	link prox;
	int N; 
};
static link head, z, list, Hlist;


link NEW(Item item, link l, link r, int N)
{
	link x = malloc(sizeof *x); 
	x->l = l; x->r = r; x->prox = NULL;
	x->item = item;
	
	if(item == NULLitem) x->qnt = 999999;
	else x-> qnt = 1;
	
	x->N = N;
	return x;
}
  
  
void STinit(){
	head = (z = NEW(NULLitem, 0, 0, 0));
	Hlist = NEW(NULLitem, 0, 0, 0);	
	printf("Inicializou estrutura: Arvores Binarias Aleatorias.\n\n");
}

int STcount() {
	return head->N;
}


link rotR(link h)
{
	link x = h->l;
	h->l = x->r;
	x->r = h;
	return x;
}

link rotL(link h)
{
	link x = h->r; 
	h->r = x->l;
	x->l = h; 
	
    return x;
}

Item searchR(link h, Key v, int add)
{
	Key t = key(h->item);
	if (h == z) return NULLitem;
    if eq(v, t){
		/*MODIFICADO*/
		h -> qnt += add;
		if(h -> qnt == 0){
			return NULLitem;
		}
		return h->item;
    }
	if less(v, t) return searchR(h->l, v, add);
	else return searchR(h->r, v, add);
}

Item STsearch(Key v, int add) 
{
	return searchR(head, v, add); 
}

link insertT(link h, Item item)
{
	Key v = key(item);
	if (h == z) return NEW(item, z, z, 1); 
	if (less(v, key(h->item))) 
	{
		h->l = insertT(h->l, item); h = rotR(h);
	}
    else
    {
		h->r = insertT(h->r, item); h = rotL(h);
	}
    return h;
}

link insertR(link h, Item item)
{	
	Key v = key(item), t = key(h->item);
	if (h == z) return NEW(item, z, z, 1);
	if (rand() < 1.0*RAND_MAX/(h->N + 1)) 
		return insertT(h, item);
	if less(v, t) h->l = insertR(h->l, item);
	else h->r = insertR(h->r, item);
	return h;
	
}
void STinsert(Item item)
{
	head = insertR(head, item);
}

link joinLR(link a, link b)
{
	if (a == z) return b;
	if (b == z) return a;
	if (1.0*rand()/RAND_MAX < 1.0*a->N/(a->N+b->N))
    {
		a->r = joinLR(a->r, b);
		return a;
	}
    else {
		b->l = joinLR(a, b->l); return b;
	}
}
  
link deleteR(link h, Key v)
{
	link x;
	Key t = key(h->item);
    if (h == z)
		return z; 
    if (less(v, t)) h->l = deleteR(h->l, v);
    if (less(t, v)) h->r = deleteR(h->r, v);
    if (eq(v, t)) 
    {
		x = h;
		h = joinLR(h->l, h->r);
		free(x);
	}
    return h;
}

void STdelete(Key v)
{
	head = deleteR(head, v); 
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

void sortR(link h, void (*visit)(Item, int))
{ 
	if (h == z) return;
	sortR(h->l, visit);
	
	if(list != NULL) list -> prox = h;
	else Hlist -> prox = h;
	
	list = h;
	list -> prox = NULL;
	
	/*visit(h->item, h->qnt);*/
	
	sortR(h->r, visit);
}
	
void STsort(void (*visit)(Item, int), int n)
{
	list = NULL;
	sortR(head, visit);
	
	mergesort(Hlist);
	list = Hlist;
	while (list -> prox != NULL){
		list = list -> prox;
		if(list -> qnt) visit(list -> item, list -> qnt);
		if(--n == 0) break;
	}
} 

void freee()
{
	link del = NULL;
	list = Hlist;
	while(list != NULL){
		free(del);
		del = list;
		list = list -> prox;
	}
	free(del);
	return;
}