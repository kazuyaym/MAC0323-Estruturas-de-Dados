#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Item.h"
#include "ST.h"

const char no_externo[] = "\\Tn";
const char no_interno[] = "\\Tc*{3pt}";
const char tree_sep[] = "3mm";
const char level_sep[] = "5mm";

typedef struct STnode* link;

struct STnode { 

	Item item;	
	int qnt;
	
	link l, r;
	link prox;
	int N, red;
};

static link head, z, list, Hlist;

link NEW(Item item, link l, link r, int N, int red)
{
	link x = malloc(sizeof *x); 
	x->l = l; x->r = r; x->prox = NULL;
	x->item = item; 
	
	if(item == NULLitem) x->qnt = 999999;
	else x-> qnt = 1;
	
	x->N = N; 
	x->red = red;
	return x;
}

void STinit()
{
	head = (z = NEW(NULLitem, 0, 0, 0, 0)); 
	Hlist = NEW(NULLitem, 0, 0, 0, 0);
	printf("Inicializou estrutura: Arvores Rubros Negras.\n\n");	
}

int STcount() 
{
	return head->N;
}

#define hl  (h->l)
#define hr  (h->r)
#define hrl  (h->r->l)
#define hll  (h->l->l)

Item searchR(link h, Key v, int add)
{
	Key t = key(h->item);
	if (h == z) return NULLitem;
	if eq(v, t){
		h -> qnt += add;
		if(h -> qnt == 0){
			return NULLitem;
		}
		return h->item;
	}
	if less(v, t) return searchR(hl, v, add);
	else return searchR(hr, v, add);
}

Item STsearch(Key v, int add) 
{	
	return searchR(head, v, add);
} 

link fixNr(link h)
{	
	h->N = hl->N + hr->N +1;
	return h;
}

link rotL(link h)
{
	link x = hr;
	hr = x->l;
	x->l = fixNr(h); 
	
    x->red = x->l->red;
	x->l->red = 1;
	
    return fixNr(x);
}

link rotR(link h)
{
	link x = hl; 
	hl = x->r;
	x->r = fixNr(h);
    
	x->red = x->r->red;
	x->r->red = 1;
    
	return fixNr(x);
}

void colorFlip(link h)
{ 
	h->red = 1 - h->red;
	hl->red = 1 - hl->red;
	hr->red = 1 - hr->red;    
}

link mvRedL(link h)
{
	colorFlip(h);
	if (hrl->red) { hr = rotR(hr); h = rotL(h); }
	return h;
}

link mvRedR(link h)
{
	colorFlip(h);
	if (hll->red) {
		h = rotR(h);
	}
	return h;
}

link balance(link h)
{
	if (hr->red) h = rotL(h);
	if (hl->red && hll->red) h = rotR(h);
	if (hl->red && hr->red) colorFlip(h);
	return fixNr(h);
}


link LLRBinsert(link h, Item item)
{	 Key v = key(item);
    /* Insert a new node at the bottom*/
    if (h == z) return NEW(item, z, z, 1, 1);  

    if (less(v, key(h->item)))
		hl = LLRBinsert(hl, item); 
    else
		hr = LLRBinsert(hr, item); 
    
    /* Enforce left-leaning condition */
    if (hr->red && !hl->red) h = rotL(h);
    if (hl->red && hll->red) h = rotR(h);
    if (hl->red && hr->red) colorFlip(h);

    return fixNr(h);
  }

void STinsert(Item item)
{
	head = LLRBinsert(head, item); head->red = 0; 
}


Item selectR(link h, int r)
{
  int t = hl->N;
  if (t > r) return selectR(hl, r);
  if (t < r) return selectR(hr, r-t-1);
  return h->item;
}

Item STselect(int r)
{
  return selectR(head, r);
}

Item getMin(link h)
{
  if (hl == z) return h->item;
  else return getMin(hl);
}

int getMinQ(link h)
{
  if (hl == z) return h->qnt;
  else return getMinQ(hl);
}

link deleteMin(link h)
{
  if (hl == z) { free(h); return z; }
  if (!hl->red && !hll->red) h = mvRedL(h);
  hl = deleteMin(hl);
  return balance(h);
}

void STdeleteMin()
{
  if (STcount() == 0) 
    { printf("Underflow"); exit(1); }

  if (!head->l->red && !head->r->red) head->red = 1;

  head = deleteMin(head);
  if (STcount() > 0) head->red = 0;
}

link deleteMax(link h)
{
  if (hl->red) h = rotR(h);
  if (hr == z) { free(h); return z; }
  if (!hr->red && !hrl->red) h = mvRedR(h);
  hr = deleteMax(hr);
  return balance(h);
}

void STdeleteMax()
{
  if (STcount() == 0) 
    { printf("Underflow"); exit(1); }

  if (!head->l->red && !head->r->red) head->red = 1;

  head = deleteMax(head);
  if (STcount() > 0) head->red = 0;
}

link deleteR(link h, Key v)
{
	Key t = key(h->item);
	if (less(v,t)) {
		if (!hl->red && !hll->red) h = mvRedL(h);
		hl = deleteR(hl, v);
	}
	else {
		if (hl->red) h = rotR(h);
		if (eq(v,key(h->item)) && hr == z)
		{
			free(h); return z;
		}
		if (!hr->red && !hrl->red) h = mvRedR(h);
		if (eq(v,key(h->item))) {
			h->item = getMin(hr);
			h->qnt = getMinQ(hr);
			hr = deleteMin(hr);
		} 
		else hr = deleteR(hr, v);
	}
	return balance(h);
}

void STdelete(Key v)
{
  if (!head->l->red && !head->r->red) head->red = 1;
  head = deleteR(head, v);
  if (STcount() > 0) head->red = 0;
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
		visit(list -> item, list -> qnt);
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