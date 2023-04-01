#pragma once
#include <SDL2/SDL_rect.h>

#define TILE_IMG_SIZE 16
#define TILE_SIZE 32

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
	TILE_BOMB = 9,
	TILE_UNDISCOVERED = 10,
	TILE_FLAGED = 11
} TileType;

class Tile {
public:
	SDL_Rect tileRect;
	SDL_Rect tileImg;
	TileType tileType;

	Tile();
private:
};