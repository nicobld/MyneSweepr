#pragma once
#include <SDL2/SDL.h>
#include <time.h>
#include <memory>
#include "state.h"

class State;

typedef enum TileType {
	TILE_BLANK = 0,
	TILE_ONE = 1,
	TILE_TWO = 2,
	TILE_THREE = 3,
	TILE_FOUR = 4,
	TILE_FIVE = 5,
	TILE_SIX = 6,
	TILE_SEVEN = 7,
	TILE_EIGHT = 8,
	TILE_BOMB = 9
} TileType;

class Tile{
public:
	Tile(TileType type);
	Tile();
	
	/* flip the tile and show what it is 
	returns -1 if not discoverable, 0 if not bomb, 1 if bomb, 2 if blank */
	int discover(State* state);

	int flag(State* state);

	TileType getTileType();

	bool getDiscovered();

	bool getFlaged();

private:
	bool discovered;
	bool flaged;
	TileType type;
};

