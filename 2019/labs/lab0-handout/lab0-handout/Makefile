CC = gcc
CFLAGS = -g -Wall -I ./include/

# This flag includes the Pthreads library on a Linux box.
# Others systems will probably require something different.
LIB = -lpthread

all: cgi tiny

tiny:
	$(CC) $(CFLAGS) -o tiny tiny.c cgi-bin/csapp.o $(LIB)

cgi:
	(cd cgi-bin; make)

clean:
	rm -rf *.o tiny exploit *.dSYM *~ *.exe
	(cd cgi-bin; make clean)

