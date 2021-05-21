IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR) `mysql_config --cflags`

ODIR=obj
_OBJ = db.o clm.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

LIBS = `mysql_config --libs` -lcurses -lpanel -lform -lmenu

_DEPS = lm.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

clm: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
