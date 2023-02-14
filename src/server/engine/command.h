#pragma once
#include <string>
#include <memory>
#include "engine.h"
#include "../state/state.h"

class State;

class Command {
public:
	State* state;

	virtual bool unserialize(std::string) = 0;
	virtual bool verify() = 0;
	virtual void execute() = 0;
};