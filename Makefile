CC=cc
CFLAGS=-Wall -Werror
LIBS=-lm -lSDL3 -lSDL3_image

.PHONY: all
all: immy

immy: immy.c
	$(CC) $(CFLAGS) -o immy immy.c $(LIBS)

install:
	install -m 755 immy /usr/bin

