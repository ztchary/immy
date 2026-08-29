CC=cc
CFLAGS=-Wall -Werror
LIBS=-lm -lSDL3 -lSDL3_image
PREFIX ?= /usr

.PHONY: all
all: immy

immy: immy.c
	$(CC) $(CFLAGS) -o immy immy.c $(LIBS)

install: immy
	install -d $(DESTDIR)$(PREFIX)/bin $(DESTDIR)$(PREFIX)/share/applications
	install -m 755 immy $(DESTDIR)$(PREFIX)/bin
	install -m 644 immy.desktop $(DESTDIR)$(PREFIX)/share/applications

