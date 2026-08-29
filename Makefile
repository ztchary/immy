CC=cc
CFLAGS=-Wall -Werror
LIBS=-lm -lSDL3 -lSDL3_image
PREFIX ?= /usr
BINDIR ?= $(PREFIX)/bin

.PHONY: all
all: immy

immy: immy.c
	$(CC) $(CFLAGS) -o immy immy.c $(LIBS)

install: immy
	install -m 755 immy $(DESTDIR)$(BINDIR)

install_desktop:
	install -m 755 immy.desktop /usr/share/applications

