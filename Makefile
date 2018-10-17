CC = gcc -g

objs = escalona.o

all: escalona

escalona: $(objs)

escalona.o: escalona.c

clean:
	-rm *.o 

purge:
	-rm escalona
