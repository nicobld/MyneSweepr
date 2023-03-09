#pragma once
#include "tile.h"
#include <vector>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Tile;

typedef enum GameState {
	GAMESTATE_PLAY,
	GAMESTATE_END,
	GAMESTATE_START,
	GAMESTATE_OPTIONS,
	GAMESTATE_PAUSE
} GameState;

typedef enum Direction {
	DIRECTION_UP,
	DIRECTION_DOWN,
	DIRECTION_LEFT,
	DIRECTION_RIGHT
} Direction;

class State {
public:
	std::vector<Tile> tiles;
	int width, height;
	GameState gameState;

	State();
	/* Return true if the state has changed */
	bool unserialize(json j);
	/* sends a GET request to server and updates state */
	void updateState(json);
	void shiftTiles(Direction direction);
private:
};