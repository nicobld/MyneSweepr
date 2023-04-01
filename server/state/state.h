#pragma once
#include "tile.h"
#include <nlohmann/json.hpp>
#include <pthread.h>

using json = nlohmann::json;

class Tile;

typedef enum GameState {
	GAMESTATE_PLAY,
	GAMESTATE_END,
	GAMESTATE_START,
	GAMESTATE_OPTIONS,
	GAMESTATE_PAUSE
} GameState;

class State {
public:
	Tile** tiles;
	int bombs;
	int flags;
	int discoveredTiles;
	int width;
	int height;
	GameState gameState;
	pthread_mutex_t lock;
	int gameID;

	State();
	void displayTiles();
	json serializeJSON();
	void reinitState();
private:
};