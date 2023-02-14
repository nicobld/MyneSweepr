#include "tile.h"
#include <memory>
#include "state.h"

Tile::Tile(TileType type){
	this->type = type;
	discovered = false;
	flaged = false;
}

Tile::Tile(){
	discovered = false;
	flaged = false;
};

int Tile::discover(State* state){
	if(discovered || flaged) return -1; //not discoverable
	state->discoveredTiles++;
	discovered = true;
	if (type == TILE_BLANK)
		return 2;
	else if (type == TILE_BOMB)
		return 1;
	else
		return 0;
}

int Tile::flag(State* state){
	if(discovered) return -1; //not flagable
	if(!flaged){
		flaged = true;
		state->flags++;
	}else {
		flaged = false;
		state->flags--;
	} 
	return 0;
}

TileType Tile::getTileType(){
	return type;
}

bool Tile::getDiscovered(){
	return discovered;
}

bool Tile::getFlaged(){
	return flaged;
}