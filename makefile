CC = gcc
CFLAGS = -c -g `mysql_config --cflags` -Wall
LIBS = `mysql_config --libs` -lcurses -lmenu -lform

OBJ = db.o string_list.o window_main.o window_insert.o

.c.o:
	$(CC) $(CFLAGS) $<

db: $(OBJ)
	$(CC) $(OBJ) $(LIBS) -o $@

db.o: string_list.h window_main.h
string_list.o: string_list.h
window_main.o: window_main.h window_insert.h
window_insert.o: window_insert.h

clean:
	rm *.o db

run:
	./db 2> errlog.txt
