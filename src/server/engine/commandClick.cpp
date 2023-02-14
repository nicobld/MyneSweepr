#include <string>
#include <bits/stdc++.h>
#include "commandClick.h"

CommandClick::CommandClick(State* state) {
	this->state = state;
}

bool CommandClick::unserialize(std::string string){
	std::stringstream stream(string);

	std::string token;

	std::vector<std::string> tokens;

	while (std::getline(stream, token, '-')){
		tokens.push_back(token);
	}

	try {
		if (tokens.size() == 4){
			column  =   (ClickType) stoi(tokens[1]);
			row	 =   (ClickType) stoi(tokens[2]);
			click   =   (ClickType) stoi(tokens[3]);
		} else {
			std::cerr << "ENGINE " << __PRETTY_FUNCTION__ << " : Invalid number of arguments\n";
			return false;
		}
	}
	catch (const std::invalid_argument& ia) {
	 std::cerr << "ENGINE " << __PRETTY_FUNCTION__ << " : Invalid argument: " << ia.what() << '\n';
		return false;
	}

	return true;
}

bool CommandClick::verify(){
	if (row > state->height || row < 0)
		return false;
	
	if (column > state->width || column < 0)
		return false;

	return true;
}

void CommandClick::execute(){
	int discoverValue;

	if (click == CLICK_LEFT){
		discoverValue = state->tiles[row][column].discover(state);

		if (discoverValue == 2){			//if discovered blank
			blankFinder(column, row);
		}
		if (discoverValue == 1){			//if discovered bomb
			state->gameState = GAMESTATE_END;
		}
	}

	else if (click == CLICK_LEFT_DOUBLE){
		if (discoverAdjTile(column, row) == 1){
			state->gameState = GAMESTATE_END;
		}
	}

	else if (click == CLICK_RIGHT){
		state->tiles[row][column].flag(state);
	}
}

void CommandClick::blankFinder(int x, int y){
	state->tiles[y][x].discover(state);

	if (y > 0){ // Check for the tile that is above
		if (state->tiles[y-1][x].getTileType() == TILE_BLANK && !state->tiles[y-1][x].getDiscovered()){
			blankFinder(x, y-1); //if it's a blank not already discovered, then blankFinder on it
		}
		if (state->tiles[y-1][x].getTileType() != TILE_BLANK){ //if it's not a blank then discover it (it's 100% not a bomb) <- ?????
	state->tiles[y-1][x].discover(state);
	if(x > 0){ //then check for the tile that is diagonaly up left (turn counter clockwise)
		if(state->tiles[y-1][x-1].getTileType() == TILE_BLANK && !state->tiles[y-1][x-1].getDiscovered()) //if its a blank, blankFinder
			blankFinder(x-1, y-1);
		else //else it's a number
			state->tiles[y-1][x-1].discover(state);
	}
			}
	}
	if (x < state->width-1){//RIGHT
		if (state->tiles[y][x+1].getTileType() == TILE_BLANK && !state->tiles[y][x+1].getDiscovered()){
			blankFinder(x+1, y);
		}
		if (state->tiles[y][x+1].getTileType() != TILE_BLANK){
			state->tiles[y][x+1].discover(state);
			if(y>0){
	if(state->tiles[y-1][x+1].getTileType() == TILE_BLANK && !state->tiles[y-1][x+1].getDiscovered())
		blankFinder(x+1, y-1);
	else
		state->tiles[y-1][x+1].discover(state);
			}
		}
	}
	if (y < state->height-1){//DOWN
		if (state->tiles[y+1][x].getTileType() == TILE_BLANK && !state->tiles[y+1][x].getDiscovered()){
			blankFinder(x, y+1);
		}
		if (state->tiles[y+1][x].getTileType() != TILE_BLANK){
			state->tiles[y+1][x].discover(state);
			if(x < state->width-1){
	if(state->tiles[y+1][x+1].getTileType() == TILE_BLANK && !state->tiles[y+1][x+1].getDiscovered())
		blankFinder(x+1, y+1);
	else
		state->tiles[y+1][x+1].discover(state);
			}
		}

	}
	if (x > 0){//LEFT
		if (state->tiles[y][x-1].getTileType() == TILE_BLANK && !state->tiles[y][x-1].getDiscovered()){
			blankFinder(x-1, y);
		}
		if (state->tiles[y][x-1].getTileType()){
			state->tiles[y][x-1].discover(state);
			if(y < state->height-1){
	if(state->tiles[y+1][x-1].getTileType() == TILE_BLANK && !state->tiles[y+1][x-1].getDiscovered())
		blankFinder(x-1, y+1);
	else
		state->tiles[y+1][x-1].discover(state);
			}
		}
	
	}
}

int CommandClick::discoverAdjTile(int x, int y){
	/* Allows the double click on discovered numbers to discover all of the adjacent ones */
	if(!hasEnoughFlags(x, y)) return 0;
	int bomb = 0;
	int temp;
	if(x > 0 && y > 0){
		if((temp = state->tiles[y-1][x-1].discover(state)) == 2) {//if discover a blank, then blankFinder() it
			blankFinder(x-1, y-1);
		} else if(temp == 1) bomb = 1; //if it's a bomb
	}
	if(x > 0){
		if((temp = state->tiles[y][x-1].discover(state)) == 2){
			blankFinder(x-1, y);
		} else if(temp == 1) bomb = 1;
	}
	if(x > 0 && y < state->height-1){
		if((temp = state->tiles[y+1][x-1].discover(state)) == 2){
			blankFinder(x-1, y+1);
		} else if(temp == 1) bomb = 1;
	}

	if(y > 0){
		if((temp = state->tiles[y-1][x].discover(state)) == 2){
			blankFinder(x, y-1);
		} else if(temp ==  1) bomb = 1;
	}
	if(y < state->height-1){
		if((temp = state->tiles[y+1][x].discover(state)) == 2){
			blankFinder(x, y+1);
		} else if(temp == 1) bomb = 1;
	}

	if(x < state->width-1 && y > 0){
		if((temp = state->tiles[y-1][x+1].discover(state)) == 2){
			blankFinder(x+1, y-1);
		} else if(temp == 1) bomb = 1;
	}
	if(x < state->width-1){
		if((temp = state->tiles[y][x+1].discover(state)) == 2){
			blankFinder(x+1, y);
		} else if(temp == 1) bomb = 1;
	}
	if(x < state->width-1 && y < state->height-1){
		if((temp = state->tiles[y+1][x+1].discover(state)) == 2){
			blankFinder(x+1, y+1);
		} else if(temp == 1) bomb = 1;
	}
	return bomb;
}


int CommandClick::hasEnoughFlags(int x, int y){
	int totFlags = 0;

	if(x > 0 && y > 0)		totFlags += state->tiles[y-1][x-1].getFlaged();
	if(x > 0)				totFlags += state->tiles[y][x-1].getFlaged();
	if(x > 0 && y < state->height-1) 		totFlags += state->tiles[y+1][x-1].getFlaged();

	if(y > 0)				totFlags += state->tiles[y-1][x].getFlaged();
	if(y < state->height-1)		totFlags += state->tiles[y+1][x].getFlaged(); 

	if(x < state->width-1 && y > 0) 		totFlags += state->tiles[y-1][x+1].getFlaged();
	if(x < state->width-1)		totFlags += state->tiles[y][x+1].getFlaged();
	if(x < state->width-1 && y < state->height-1) totFlags += state->tiles[y+1][x+1].getFlaged();

	if(totFlags == state->tiles[y][x].getTileType()) return 1;
	return 0;
}
