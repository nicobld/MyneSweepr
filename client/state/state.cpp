#include "state.h"
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include "../client/client.h"

State::State() {
	width = 0;
	height = 0;
	gameState = GAMESTATE_PLAY;
	gameID = 0;

	tiles.resize(width*height);

	Tile tile;
	for (int j = 0; j < height; j++){
		for (int i = 0; i < width; i++){
			tile = tiles[j*height + i];
			tile.tileType = TILE_UNDISCOVERED;
			tile.tileImg = {tile.tileType*TILE_IMG_SIZE, 0, TILE_IMG_SIZE, TILE_IMG_SIZE};
			tile.tileRect = {i*TILE_SIZE, j*TILE_SIZE, TILE_SIZE, TILE_SIZE};
		}
	}
}

bool State::unserialize(json js){
	json tiles_json;
	bool new_game = false;
	try {
		gameState = js["gameState"];
		tiles_json = js["tiles"];
		width = js["width"];
		height = js["height"];
		int id;
		if ((id = js["gameID"]) != gameID){
			new_game = true;
			gameID = id;
		}
	} catch (const std::exception&) {
		return false;
	}

	if (new_game){
		tiles.resize(width*height);
	}

	for (int j = 0; j < height; j++){
		for (int i = 0; i < width; i++){
			tiles[j*height + i].tileType = (TileType) tiles_json.at(j*height + i).get<int>();
		}
	}

	return new_game;
}

void State::updateState(json js){

	bool new_game = unserialize(js);

	Tile* tile;
	for (int j = 0; j < height; j++){
		for (int i = 0; i < width; i++){
			tile = &tiles[j*height + i];
			tile->tileImg = {(int)tile->tileType*TILE_IMG_SIZE, 0, TILE_IMG_SIZE, TILE_IMG_SIZE};
			if (new_game) tile->tileRect = {i*TILE_SIZE, j*TILE_SIZE, TILE_SIZE, TILE_SIZE};
		}
	}
}

void State::shiftTiles(Direction dir){
	for (int j = 0; j < height; j++){
		for (int i = 0; i < width; i++){
			tiles[j*height + i].tileRect.x += ((dir == DIRECTION_RIGHT ? -1 : 0) + (dir == DIRECTION_LEFT ? 1 : 0)) * TILE_SIZE*2.5;
			// printf("x: %d\n", ((dir == DIRECTION_RIGHT ? 1 : 0) + (dir == DIRECTION_LEFT ? -1 : 0)) * TILE_SIZE/2 );
			tiles[j*height + i].tileRect.y += ((dir == DIRECTION_UP ? 1 : 0) + (dir == DIRECTION_DOWN ? -1 : 0)) * TILE_SIZE*2.5;
		}
	}
}