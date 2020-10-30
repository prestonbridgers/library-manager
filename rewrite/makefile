CC = gcc
CFLAGS = -c -g -Wall

LIBS = -lcurses -lpanel -lform -lmenu
OBJ = main.o clm.o

.c.o:
	$(CC) $(CFLAGS) $<

clm: $(OBJ)
	$(CC) $(OBJ) $(LIBS) -o $@

main.o: clm.h
clm.o: clm.h

clean:
	rm clm *.o

run:
	make
	./clm library book 2> err.log
	make clean
