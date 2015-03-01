all : simpletest mytest

simpletest : simpletest.o smalloc.o testhelpers.o
	gcc -Wall -g -o simpletest simpletest.o smalloc.o testhelpers.o

%.o : %.c smalloc.h
	gcc -Wall -g -c $<

clean :
	rm simpletest mytest *.o

tests : mytest simpletest
	./simpletest
	./mytest

mytest : mytest.o simpletest.o smalloc.o testhelpers.o
	gcc -Wall -g -o mytest mytest.o smalloc.o testhelpers.o
