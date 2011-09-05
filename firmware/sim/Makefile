UNAME := $(shell uname)


all:
ifeq ($(UNAME), Darwin)
	gcc main.c $(APP).c -o $(APP) -I/Library/Frameworks/SDL.framework/Headers SDLmain.m -framework SDL -framework Cocoa
endif
ifeq ($(UNAME), Linux)
	gcc main.c $(APP).c -o $(APP) -lSDL
endif
