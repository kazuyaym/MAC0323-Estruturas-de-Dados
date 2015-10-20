/*
 * This code is from "Algorithms in C, Third Edition,"
 * by Robert Sedgewick, Addison Wesley Longman, 1998.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Item.h"
static char buf[10000000];
static int cnt = 0;

int ITEMscan(char **x)
{
	int t;
    *x = &buf[cnt]; 
	printf("%i,\n", cnt);
    t = scanf("%s", *x);
	cnt += strlen(*x)+1;
	printf("%i,\n", cnt);
    return t;
}  

void ITEMshow(char *x) 
{
	printf("%s\n", x);
}
