#
VER = 0.1
PROG = biorythm
PREFIX = tmp

CC=gcc
CPP=g++
LIBS= -lm

CCODE = $(shell ls *.c)
HEAD = $(shell ls *.h)
OBJET = $(shell ls *.c | sed -e 's/\.c$//\.o/') 

all: $(PROG)

$(PROG): $(OBJET)
	$(CPP) -o $@ $^ $(LDFLAGS) $(LIBS)

%.o: %.c $(HEAD)
	$(CC) $(CFLAGS) -c -Wall -o $@ $< 

clean:
	rm -f *.o *~ $(PROG)

cleanzip: cleantmp
	rm -f $(PREFIX)/$(PROG).zip

cleantmp:
	rm -rf $(PREFIX)

zip: all cleanzip cleantmp
	mkdir -p $(PREFIX)/$(PROG)
	cp biorythm.c date.c date.h Makefile README.md $(PREFIX)/$(PROG)/
	cd $(PREFIX); zip -r $(PROG) $(PROG)/*
	unzip -l $(PREFIX)/$(PROG).zip
