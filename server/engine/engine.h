#pragma once
#include <queue>
#include <string>
#include <memory>
#include "../state/state.h"
#include "command.h"

class Command;
class State;

enum ClickType {
	CLICK_LEFT,
	CLICK_LEFT_DOUBLE,
	CLICK_RIGHT,
	CLICK_RIGHT_DOUBLE
};

class Engine {
public:
	std::queue<std::unique_ptr<Command>> commandQueue;
	State* state;

	Engine();
	void pushCommand(std::string);
	void* update();
	bool checkIfWon();
	void initTiles();
};