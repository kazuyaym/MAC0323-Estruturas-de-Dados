all: driver

driver: driver.o ST.o Item.o
	gcc -o driver driver.o ST.o Item.o -Wall -pedantic -ansi

driver.o: driver.c
	gcc -c driver.c
	
ST.o: ST.c
	gcc -c ST.c

Item.o: Item.c
	gcc -c Item.c