UNAME := $(shell uname)


all:
ifeq ($(UNAME), Darwin)
	gcc sim.c $(APP).c -o $(APP) -I/Library/Frameworks/SDL.framework/Headers SDLmain.m -framework SDL -framework Cocoa
endif
ifeq ($(UNAME), Linux)
	gcc sim.c $(APP).c -o $(APP) -lSDL
endif
