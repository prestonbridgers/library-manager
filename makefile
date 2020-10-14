CC = gcc
CFLAGS = -c -g `mysql_config --cflags` -Wall
LIBS = `mysql_config --libs` -lcurses -lmenu

OBJ = ui.o window_main.o

.c.o:
	$(CC) $(CFLAGS) $<

db: $(OBJ)
	$(CC) $(OBJ) $(LIBS) -o $@

ui.o: window_main.h
window_main.o: window_main.h

clean:
	rm *.o db

run:
	./db 2> errlog.txt
