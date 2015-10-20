#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Item.h"

static char buf[10000000];
static int cnt = 0;


int ITEMscan(char **x, char *entrada)
{
	*x = &buf[cnt]; 
	sscanf(entrada,"%s", *x);
	cnt += strlen(*x)+1;
	return 0;
}  

void ITEMshow(char *x, int q) 
{
	printf("%s %i\n", x, q);
}