typedef char *Item;
typedef char *Key;
#define key(A) (A)
#define eq(A, B) (strcmp(A,B)==0)
#define less(A, B) (strcmp(A,B)<0)
#define NULLitem (NULL)

 int ITEMscan(Item *, Key);
void ITEMshow(Item, int);