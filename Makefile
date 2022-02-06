LIBS = -lSDL2main -lSDL2_image -lSDL2
FLAGS = -Wall
FILES = src/main.cpp src/tile.cpp src/functions.cpp includes/main.h includes/tile.h includes/common.h includes/functions.h

prog: $(FILES)
	-g++ $(FLAGS) $(FILES) -o $@ $(LIBS)

	