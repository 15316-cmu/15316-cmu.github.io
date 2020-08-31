CC = gcc
CFLAGS = -g -Wall -std=c99 -I ../include/
SCRIPTS = adder divide store get
LIB = -lpthread
all: extendible_hash.o csapp.o ubarray.o adder divide store get

adder: adder.c
	$(CC) $(CFLAGS) -o adder adder.c

divide: divide.c
	$(CC) $(CFLAGS) -o divide divide.c

store:  store.c csapp.o extendible_hash.o ubarray.o
	$(CC) $(CFLAGS) -o store store.c csapp.o extendible_hash.o ubarray.o $(LIB)

get: get.c csapp.o extendible_hash.o ubarray.o
	$(CC) $(CFLAGS) -o get get.c csapp.o extendible_hash.o ubarray.o $(LIB)

extendible_hash.o: extendible_hash.c
	$(CC) $(CFLAGS) -c extendible_hash.c

csapp.o: csapp.c
	$(CC) -g -O2 -Wall -I ../include/ -c csapp.c

ubarray.o: ubarray.c
	$(CC) $(CFLAGS) -c ubarray.c

clean:
	rm -rf $(SCRIPTS) *.o *.dSYM *~ *.exe
