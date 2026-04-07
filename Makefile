.PHONY: clean

CC=gcc
INCLUDE=include
SRC=src
OBJS=main.o curso.o discente.o turma.o

sigac: $(OBJS)
	gcc -o sigac $(OBJS)

main.o: $(SRC)/main.c
	gcc -c $(SRC)/main.c -I$(INCLUDE)

curso.o: $(SRC)/curso.c
	gcc -c $(SRC)/curso.c -I$(INCLUDE)

discente.o: $(SRC)/discente.c
	gcc -c $(SRC)/discente.c -I$(INCLUDE)

turma.o: $(SRC)/turma.c
	gcc -c $(SRC)/turma.c -I$(INCLUDE)

clean:
	-rm *.csv
	rm sigac
	rm *.o
