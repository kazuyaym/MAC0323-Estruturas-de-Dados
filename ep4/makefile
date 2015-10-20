# ***************************************************************
#
# Makefile genérico para compilar programas em C
# usando o compilador gcc
#
# ***************************************************************

CFLAGS = -Wall -pedantic -ansi

CC = gcc

#======================================================================

.c.o:
	$(CC) $(CFLAGS) -c $*.c

.c:
	make $*.o
	$(CC) $(CFLAGS) -o $* $*.o $(LDLIBS) 

#======================================================================

clean:
	del *.o

#======================================================================

ep4SL: clean ep4main.o SL.o Item.o
	$(CC) $(CFLAGS) -o epSL ep4main.o SL.o Item.o
	
ep4BST: clean ep4main.o BST.o Item.o
	$(CC) $(CFLAGS) -o epBST ep4main.o BST.o Item.o
	
ep4rBST: clean ep4main.o rBST.o Item.o
	$(CC) $(CFLAGS) -o eprBST ep4main.o rBST.o Item.o
	
ep4LLRB: clean ep4main.o LLRB.o Item.o
	$(CC) $(CFLAGS) -o epLLRB ep4main.o LLRB.o Item.o
	
ep4main.o: ep4main.c
	$(CC) $(CFLAGS) -c ep4main.c
	
SL.o: 
	$(CC) $(CFLAGS) -c SL.c
	
BST.o: 
	$(CC) $(CFLAGS) -c BST.c
	
rBST.o: 
	$(CC) $(CFLAGS) -c rBST.c
	
LLRB.o: 
	$(CC) $(CFLAGS) -c LLRB.c

Item.o:
	$(CC) $(CFLAGS) -c Item.c
