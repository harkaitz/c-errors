.POSIX:
.SUFFIXES:
.PHONY: all clean install check

PROJECT   =c-errors
VERSION   =1.0.0
CC        =$(shell which $(TPREFIX)cc $(TPREFIX)gcc 2>/dev/null | head -n 1)
CFLAGS    =-Wall -g3 -std=c99
PREFIX    =/usr/local
BUILDDIR ?=.build
UNAME_S  ?=$(shell uname -s)
EXE      ?=$(shell uname -s | awk '/Windows/ || /MSYS/ || /CYG/ { print ".exe" }')
PROGS     =$(BUILDDIR)/slog$(EXE)

all: $(PROGS)
clean:
	rm -f $(PROGS)
install:
	mkdir -p $(DESTDIR)$(PREFIX)/bin $(DESTDIR)$(PREFIX)/include
	cp $(PROGS) $(DESTDIR)$(PREFIX)/bin
	cp errors.h $(DESTDIR)$(PREFIX)/include
check:


$(BUILDDIR)/slog$(EXE): examples/slog.c errors.h
	mkdir -p $(BUILDDIR)
	$(CC) -o $@ examples/slog.c $(CFLAGS) $(CPPFLAGS)

## -- BLOCK:c --
clean: clean-c
clean-c:
	rm -f *.o
## -- BLOCK:c --
