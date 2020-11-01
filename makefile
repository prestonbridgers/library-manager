CC = gcc
CFLAGS = -c -g -Wall `mysql_config --cflags`

LIBS = `mysql_config --libs` -lcurses -lpanel -lform -lmenu
OBJ = main.o clm.o db.o

.c.o:
	$(CC) $(CFLAGS) $<

clm: $(OBJ)
	$(CC) $(OBJ) $(LIBS) -o $@

main.o: lm.h
clm.o: lm.h
db.o: lm.h

clean:
	rm clm *.o

run:
	make
	./clm library book -t 2> err.log
	make clean
