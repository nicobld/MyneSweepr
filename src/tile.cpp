#include "../includes/tile.h"


Tile::Tile(int x, int y, Type type){
	this->type = type;
	tileRect.x = x;
	tileRect.y = y;
	tileRect.w = TILE_SIZE;
	tileRect.h = TILE_SIZE;
}

Tile::Tile(){};

void Tile::setTileImg(int x, int y){
	tileImg.x = x;
	tileImg.y = y;
}

int Tile::discover(){
	/* flip the tile and show what it is 
	returns -1 if not discoverable, 0 if not bomb, 1 if bomb */
	if(discovered || flaged) return 0; //not discoverable
	if(firstDiscover){
		if (type != BLANK){
			return -1;
		}
	}
	firstDiscover = 0;
	discoveredTiles++;
	discovered = 1;
	switch(type){
		case ONE:
			setTileImg(0,0);
			break;
		case TWO:
			setTileImg(16,0);
			break;
		case THREE:
			setTileImg(32,0);
			break;
		case FOUR:
			setTileImg(48,0);
			break;
		case FIVE:
			setTileImg(64,0);
			break;
		case SIX:
			setTileImg(80,0);
			break;
		case SEVEN:
			setTileImg(96,0);
			break;
		case EIGHT:
			setTileImg(112,0);
			break;
		case BLANK:
			setTileImg(32,16);
			return 2; //blank
		case BOMB:
			setTileImg(48,16);
			return 1; //it's a bomb
	}return 0; //it's not bomb
}

int Tile::flag(){
	if(discovered) return -1; //not flagable
	if(!flaged){
		flaged = 1;
		setTileImg(0,16);
		flags++;
	}else {
		flaged = 0;
		setTileImg(16,16);
		flags--;
	}return 0;
}

Type Tile::getType(){
	return type;
}

SDL_Rect Tile::getTileRect(){
	return tileRect;
}

SDL_Rect Tile::getTileImg(){
	return tileImg;
}

int Tile::getDiscovered(){
	return discovered;
}

int Tile::getFlaged(){
	return flaged;
}