# Makefile for itinerary engine utilities
# $Header$

CC= gcc
CFLAGS= -g
LD= gcc
LDFLAGS= -g 
HOMEDIR= /usr/local/etc/gild
TARGETS= gild
COMPONENTS = gild.o wrapper.o config.o log.o

all: $(TARGETS)

.c.o:
	$(CC) $(CFLAGS) -c $<

gild: $(COMPONENTS) gild.h Makefile 
	$(LD) $(LDFLAGS) -o gild $(COMPONENTS)

clean:
	rm core *.o $(TARGETS)

install: $(TARGETS)
	install --strip $(TARGETS) $(HOMEDIR)
	install gild.conf $(HOMEDIR)
	cd handlers; make install

version: $(TARGETS)
	cvs commit gild.c wrapper.c config.c log.c Makefile gild.h \
	    gild.conf handlers