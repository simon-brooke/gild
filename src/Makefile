# Makefile for Generic Internet Listener Daemon
# $Header$

CC= gcc
CFLAGS= -g
LD= gcc
LDFLAGS= -g 
HOMEDIR= /usr/local/etc/gild
TARGETS= ../bin/gild
COMPONENTS = gild.o wrapper.o config.o log.o

all: $(TARGETS)

.c.o:
	$(CC) $(CFLAGS) -c $<

../bin/gild: $(COMPONENTS) gild.h Makefile 
	$(LD) $(LDFLAGS) -o gild $(COMPONENTS)
	mv gild ../bin

clean:
	rm core $(COMPONENTS) $(TARGETS)

install: $(TARGETS)
	install --strip $(TARGETS) $(HOMEDIR)
	install gild.conf $(HOMEDIR)
	cd handlers; make install

version: $(TARGETS)
	cvs commit gild.c wrapper.c config.c log.c Makefile gild.h \
	    gild.conf handlers
