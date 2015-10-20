/*
 * This code is from "Algorithms in C, Third Edition,"
 * by Robert Sedgewick, Addison Wesley Longman, 1998.
 */
typedef char *Item;
typedef char *Key;
#define key(A) (A)
#define eq(A, B) (strcmp(A,B)==0)
#define less(A, B) (strcmp(A,B)<0)
#define NULLitem (NULL)

int ITEMscan(Item *);
void ITEMshow(Item);