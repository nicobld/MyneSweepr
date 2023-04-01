#include <iostream>
#include <stdio.h>
#include <vector>

#include <nlohmann/json.hpp>
#include "tile.h"
#include "state.h"
#include "../common.h"

using json = nlohmann::json;

class Tile;

State::State(){
	bombs = 100;
	flags = 0;
	discoveredTiles = 0;
	width = 30;
	height = 30;
	gameState = GAMESTATE_START;
	while ((gameID = rand()) == 0);
	if (pthread_mutex_init(&lock, NULL) == -1){
		error_and_die("pthread_mutex_init\n");
	}
}

void State::displayTiles(){
	int i, j;
	
	for (i = 0; i < width + 2; i++)
		printf("%%");
	printf("\n");

	for(j = 0; j < width; j++){
		for(i = 0; i < height; i++){
			if (i == 0) printf("%%");

			if (tiles[j][i].getDiscovered() == true){
				switch (tiles[j][i].getTileType()){
					case TILE_BLANK:
						printf(" ");
						break;
					case TILE_BOMB:
						printf("x");
						break;
					case TILE_ONE:
						printf("1");
						break;
					case TILE_TWO:
						printf("2");
						break;
					case TILE_THREE:
						printf("3");
						break;
					case TILE_FOUR:
						printf("4");
						break;
					case TILE_FIVE:
						printf("5");
						break;
					case TILE_SIX:
						printf("6");
						break;
					case TILE_SEVEN:
						printf("7");
						break;
					case TILE_EIGHT:
						printf("8");
						break;
				}
			} else {
				if (tiles[j][i].getFlaged() == true){
					printf("?");
				}
				else {
					printf("#");
				}

			}

			if (i == width - 1) printf("%%");
		}
		printf("\n");
	}

	for (i = 0; i < width + 2; i++)
		printf("%%");
	printf("\n");

}

json State::serializeJSON(){
	json j;

	pthread_mutex_lock(&lock);

	j["width"] = width;
	j["height"] = height;
	j["gameState"] = gameState;
	j["gameID"] = gameID;

	std::vector<int> vec;
	for (int j = 0; j < height; j++){
		for (int i = 0; i < width; i++){
			if (tiles[j][i].getDiscovered())
				vec.push_back((int)tiles[j][i].getTileType());
			else
				if (tiles[j][i].getFlaged())
					vec.push_back(11);
				else
					vec.push_back(10);
		}
	}
	j["tiles"] = vec;
	pthread_mutex_unlock(&lock);
	return j;
}