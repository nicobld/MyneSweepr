#include "tile.h"
#include <SDL2/SDL_rect.h>

Tile::Tile() {
	tileImg = {TILE_UNDISCOVERED*TILE_IMG_SIZE, TILE_UNDISCOVERED*TILE_IMG_SIZE, TILE_IMG_SIZE, TILE_IMG_SIZE};
}