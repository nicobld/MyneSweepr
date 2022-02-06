LIBS = -lSDL2main -lSDL2
FLAGS = -Wall
FILES = src/* includes/*

prog: $(FILES)
	-g++ $(FLAGS) $(FILES) -o $@ $(LIBS)