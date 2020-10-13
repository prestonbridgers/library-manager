CC = gcc
CFLAGS = -c -g `mysql_config --cflags` -Wall
LIBS = `mysql_config --libs` -lcurses -lmenu

OBJ = ui.o

.c.o:
	$(CC) $(CFLAGS) $<

db: $(OBJ)
	$(CC) $(OBJ) $(LIBS) -o $@

main.o: 
ui.o: 

clean:
	rm *.o *.out
