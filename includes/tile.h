#ifndef TILE_H
#define TILE_H

#include <SDL2/SDL.h>
#include <time.h>
#include <cstring>
#include "common.h"

extern int flags;
extern int discoveredTiles;
extern int firstDiscover;

typedef enum Type{
	BLANK = 0,
	ONE = 1,
	TWO = 2,
	THREE = 3,
	FOUR = 4,
	FIVE = 5,
	SIX = 6,
	SEVEN = 7,
	EIGHT = 8,
	BOMB = 9
}Type;


class Tile{
public:
	Tile(int x, int y, Type type);
	Tile();
	int discover();
	int flag();
	void setTileImg(int x, int y);
	Type getType();
	SDL_Rect getTileRect();
	SDL_Rect getTileImg();
	int getDiscovered();
	int getFlaged();

private:
	SDL_Rect tileRect;
	SDL_Rect tileImg = {16,16,TILE_IMG_SIZE,TILE_IMG_SIZE};
	int discovered = 0;
	int flaged = 0;
	Type type;
};

extern Tile** tiles;


#endif