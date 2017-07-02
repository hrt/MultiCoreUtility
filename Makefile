CC			= gcc
CFLAGS  = -std=c99 -Wall -Werror -pedantic
OBJDIR	= obj
SRCDIR	= src
OUTDIR	= bin

_DEPS		= util.h
DEPS    = $(patsubst %,$(SRCDIR)/%,$(_DEPS))

_OBJ		= main.o util.o
OBJ			= $(patsubst %,$(OBJDIR)/%,$(_OBJ))

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


$(OUTDIR)/ShmMultiThread: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean check all

clean:
	rm -f $(OBJDIR)/*.o $(OUTDIR)/ShmMultiThread