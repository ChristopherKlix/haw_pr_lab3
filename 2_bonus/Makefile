main: main.o util.o
	cc main.o util.o -o main

main.o: main.c
	cc -c main.c

util.o: util.c util.h
	cc -c util.c

clean:
	rm *.o main
