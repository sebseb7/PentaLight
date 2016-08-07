UNAME := $(shell uname)

SOURCES=$(wildcard ../apps/*.c)
BINARIES=$(SOURCES:../apps/%.c=%)

LIB_SOURCES=$(wildcard ../libs/*.c)
LIB_HEADERS=$(wildcard ../libs/*.h)

LIB_SOURCES+= sdl_draw/SDL_draw.c
LIB_HEADERS+= sdl_draw/SDL_draw.h


FLAGS=--std=gnu99 -Wall

FLAGS+= `sdl-config --cflags`
LDFLAGS+= `sdl-config --libs`

all: $(BINARIES)

clean:
	rm -f ${BINARIES}

%: ../apps/%.c main.c main.h Makefile $(LIB_HEADERS) $(LIB_SOURCES)
	gcc $(FLAGS) main.c $< -o $@ -I. $(LIB_SOURCES) -I../libs $(LDFLAGS)
