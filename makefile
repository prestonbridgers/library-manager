CC = gcc
CFLAGS = -c -g `mysql_config --cflags` -Wall
LIBS = `mysql_config --libs` -lcurses -lmenu -lform -lpanel

OBJ = util.o db.o lm.o string_list.o window_main.o window_insert.o

.c.o:
	$(CC) $(CFLAGS) $<

lm: $(OBJ)
	$(CC) $(OBJ) $(LIBS) -o $@

lm.o: library_manager.h
string_list.o: string_list.h
window_main.o: library_manager.h
window_insert.o: library_manager.h
util.o: library_manager.h

clean:
	rm *.o lm errlog.txt

run:
	./lm 2> errlog.txt
