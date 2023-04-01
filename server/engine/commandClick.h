#pragma once
#include "command.h"
#include "engine.h"

class State;

class CommandClick : public Command {
public:
	CommandClick(State*);
	/* format: "click-<column>-<row>-<click>" */
	bool unserialize(std::string) override;
	bool verify() override;
	void execute() override;
private:
	int row, column;
	enum ClickType click; //0: left click, 1: double left click, 2: right click
	void blankFinder(int, int);
	int discoverAdjTile(int x, int y);
	int hasEnoughFlags(int x, int y);
};