# Makefile for Generic Internet Listener Daemon
# $Header$

CC= gcc
CFLAGS= -g
LD= gcc
LDFLAGS= -g 
HOMEDIR= /usr/local/etc/gild
BINDIR= ../bin
TARGETS= $(BINDIR)/gild
COMPONENTS = gild.h gild.o wrapper.o config.o log.o
VERSION= $(shell git describe)

all: $(TARGETS)

.c.o:
	$(CC) $(CFLAGS) -c $<

gild.h: gild.h.src
	sed "s?@Revision@?$(VERSION)?" gild.h.src > gild.h

../bin/gild: $(COMPONENTS) gild.h Makefile 
	$(LD) $(LDFLAGS) -o gild $(COMPONENTS)
	test -d $(BINDIR) || mkdir $(BINDIR)
	mv gild $(BINDIR)

clean:
	rm -f core $(COMPONENTS) $(TARGETS)

install: $(TARGETS)
	install --strip $(TARGETS) $(HOMEDIR)
	install gild.conf $(HOMEDIR)
	cd handlers; make install


