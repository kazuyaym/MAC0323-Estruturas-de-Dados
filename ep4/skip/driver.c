/*
 * This code is from "Algorithms in C, Third Edition,"
 * by Robert Sedgewick, Addison Wesley Longman, 1998.
 * (With small alterations)
 */
#include <stdio.h>
#include <stdlib.h>
#include "Item.h"
#include "ST.h"

int main()
 { 
	int N; 
	Key v; Item item;
	STinit();
	for (N = 0; ITEMscan(&v) != EOF; N++)
	{	
		if (STsearch(v) == NULLitem)
		{ 
			key(item) = v; STinsert(item);
		}
	}
    
	STsort(ITEMshow); printf("\n");
    printf("%d keys, ", N);
    printf("%d distinct keys\n", STcount());
    return 0;
 }
