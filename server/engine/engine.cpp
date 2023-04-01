#include <bits/stdc++.h>
#include <unistd.h>

#include <iostream>
#include <memory>
#include <string>

#include "engine.h"
#include "command.h"
#include "commandClick.h"

Engine::Engine(){}

void Engine::pushCommand(std::string string){
	std::stringstream stream(string);
	std::string commandType;

	// std::cout << "new command: \"" << string << "\"" << std::endl;

	std::getline(stream, commandType, '-');

	if (commandType == "click"){
		std::unique_ptr<CommandClick> commandClick = std::make_unique<CommandClick>(state);
		if (commandClick->unserialize(string))
			commandQueue.emplace(std::move(commandClick));
	}
}

void* Engine::update(){
	bool quit = false;

	while(!quit){
		if(state->gameState == GAMESTATE_START){
			initTiles();
			state->gameState = GAMESTATE_PLAY;
		}

		if(state->gameState == GAMESTATE_OPTIONS){

		}

		if(state->gameState == GAMESTATE_PLAY){
			while(commandQueue.size() > 0){
				auto command = std::move(commandQueue.front());
				pthread_mutex_lock(&state->lock);
				if (command->verify()){
					command->execute();
				}
				pthread_mutex_unlock(&state->lock);
				commandQueue.pop();
				// state->displayTiles();
			}

			if (checkIfWon())
				state->gameState = GAMESTATE_END;
		}


		if(state->gameState == GAMESTATE_END){
			printf("game has ended");
			pthread_mutex_lock(&state->lock);
			state->flags = 0;
			state->discoveredTiles = 0;
			for(int i = 0; i < state->height; i++)
				free(state->tiles[i]);
			free(state->tiles);
			state->gameState = GAMESTATE_START;
			pthread_mutex_unlock(&state->lock);
		}

		if(state->gameState == GAMESTATE_PAUSE){

		}

		usleep(1000);
	}
	return NULL;
}


void Engine::initTiles(){
	pthread_mutex_lock(&state->lock);
	state->tiles = (Tile**) malloc(sizeof (Tile*) * state->height);
	for(int i = 0; i < state->height; i++)
		state->tiles[i] = (Tile*) malloc(sizeof (Tile) * state->width);

	int bombTable[state->height][state->width];
	memset(bombTable, 0, sizeof(bombTable));

	int randHeight;
	int randWidth;

	for(int i = 0; i < state->bombs; i++){
		randHeight = rand()%state->height;
		randWidth = rand()%state->width;
		if(bombTable[randHeight][randWidth] == 1)
			i--;
		else
			bombTable[randHeight][randWidth] = 1;
	}

	int bombCount = 0;
	for(int y = 0; y < state->height; y++){
		for(int x = 0; x < state->width; x++){
			if(!bombTable[y][x]){
				if(x > 0 && y > 0)								bombCount += bombTable[y-1][x-1];
				if(x > 0)										bombCount += bombTable[y][x-1];
				if(x > 0 && y < state->height-1)				bombCount += bombTable[y+1][x-1];

				if(y > 0)										bombCount += bombTable[y-1][x];
				if(y < state->height-1)							bombCount += bombTable[y+1][x]; 

				if(x < state->width-1 && y > 0)					bombCount += bombTable[y-1][x+1];
				if(x < state->width-1)							bombCount += bombTable[y][x+1];
				if(x < state->width-1 && y < state->height-1)	bombCount += bombTable[y+1][x+1];

				state->tiles[y][x] = Tile((TileType)bombCount);
			} else {
				state->tiles[y][x] = Tile(TILE_BOMB);
			}
			bombCount = 0;
		}
	}
	pthread_mutex_unlock(&state->lock);
}

bool Engine::checkIfWon(){
	pthread_mutex_lock(&state->lock);
	if(state->flags == state->bombs && state->flags + state->discoveredTiles == state->width * state->height){
		std::cout << "You won !" << std::endl;
		pthread_mutex_unlock(&state->lock);
		return true;
	}
	pthread_mutex_unlock(&state->lock);
	return false;
}