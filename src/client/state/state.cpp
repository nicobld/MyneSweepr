#include "state.h"
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include "../client/client.h"

State::State() {
	width = 20;
	height = 20;
	gameState = GAMESTATE_PLAY;

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
	bool changed = false;
	
	for (auto it = js.begin(); it != js.end(); it++){
		if (it.key() == "width"){
			if (width != it.value())
				changed = true;
			width = it.value();
		}
		else if (it.key() == "height"){
			if (height != it.value()) 
				changed = true;
			height = it.value();
		}
		else if (it.key() == "gameState"){
			gameState = it.value();
		}
		else if (it.key() == "tiles"){
			tiles_json = it.value();
		}
	}

	if (changed){
		tiles.resize(width*height);
	}

	for (int j = 0; j < height; j++){
		for (int i = 0; i < width; i++){
			tiles[j*height + i].tileType = (TileType) tiles_json.at(j*height + i).get<int>();
		}
	}

	return true;
}

void State::updateState(){
	char* body;
	char uri[BUFSIZE];
	json js;

	strcpy(uri, "/state ");

	if ((body = http_get(uri)) == NULL){
		printf("body error\n");
		return;
	}

	js = json::parse(body);
	unserialize(js);

	Tile* tile;
	for (int j = 0; j < height; j++){
		for (int i = 0; i < width; i++){
			tile = &tiles[j*height + i];
			tile->tileImg = {(int)tile->tileType*TILE_IMG_SIZE, 0, TILE_IMG_SIZE, TILE_IMG_SIZE};
			tile->tileRect = {i*TILE_SIZE, j*TILE_SIZE, TILE_SIZE, TILE_SIZE};
		}
	}
}