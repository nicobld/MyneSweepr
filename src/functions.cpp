#include "../includes/functions.h"

void initTiles(Tile** tiles){
	srand(time(NULL));
	int bt[height][width]; //bombTable
	memset(bt,0,sizeof(bt));

	int randHeight;
	int randWidth;

	for(int i=0;i<bombs;i++){
		randHeight = rand()%height;
		randWidth = rand()%width;
		if(bt[randHeight][randWidth]==1)
			i--;
		else
			bt[randHeight][randWidth] = 1;
	}

	int bombCount=0;
	for(int y=0;y<height;y++){
		for(int x=0;x<width;x++){
			if(!bt[y][x]){
				if(x>0 && y>0) 				bombCount += bt[y-1][x-1];
				if(x>0) 					bombCount += bt[y][x-1];
				if(x>0 && y<height-1) 		bombCount += bt[y+1][x-1];

				if(y>0) 					bombCount += bt[y-1][x];
				if(y<height-1) 				bombCount += bt[y+1][x]; 

				if(x<width-1 && y>0) 		bombCount += bt[y-1][x+1];
				if(x<width-1) 				bombCount += bt[y][x+1];
				if(x<width-1 && y<height-1) bombCount += bt[y+1][x+1];

				tiles[y][x] = Tile(x*TILE_SIZE,y*TILE_SIZE,(Type)bombCount);
			} else {
				tiles[y][x] = Tile(x*TILE_SIZE,y*TILE_SIZE,BOMB);
			}
			bombCount=0;
		}
	}
}

void blankFinder(Tile** tiles, int x, int y){
	tiles[y][x].discover();

	if (y>0){ // Check for the tile that is above
		if (tiles[y-1][x].getType()==BLANK && !tiles[y-1][x].getDiscovered()){
			blankFinder(tiles,x,y-1); //if it's a blank not already discovered, then blankFinder on it
		}
		if (tiles[y-1][x].getType()!=BLANK){ //if it's not a blank then discover it (it's 100% not a bomb)
				tiles[y-1][x].discover();
				if(x>0){ //then check for the tile that is diagonaly up left (turn counter clockwise)
					if(tiles[y-1][x-1].getType()==BLANK && !tiles[y-1][x-1].getDiscovered()) //if its a blank, blankFinder
						blankFinder(tiles,x-1,y-1);
					else //else it's a number
						tiles[y-1][x-1].discover();
				}
			}
	}
	if (x<width-1){//RIGHT
		if (tiles[y][x+1].getType()==BLANK && !tiles[y][x+1].getDiscovered()){
			blankFinder(tiles,x+1,y);
		}
		if (tiles[y][x+1].getType()!=BLANK){
			tiles[y][x+1].discover();
			if(y>0){
				if(tiles[y-1][x+1].getType()==BLANK && !tiles[y-1][x+1].getDiscovered())
					blankFinder(tiles,x+1,y-1);
				else
					tiles[y-1][x+1].discover();
			}
		}
	}
	if (y<height-1){//DOWN
		if (tiles[y+1][x].getType()==BLANK && !tiles[y+1][x].getDiscovered()){
			blankFinder(tiles,x,y+1);
		}
		if (tiles[y+1][x].getType()!=BLANK){
			tiles[y+1][x].discover();
			if(x<width-1){
				if(tiles[y+1][x+1].getType()==BLANK && !tiles[y+1][x+1].getDiscovered())
					blankFinder(tiles,x+1,y+1);
				else
					tiles[y+1][x+1].discover();
			}
		}

	}
	if (x>0){//LEFT
		if (tiles[y][x-1].getType()==BLANK && !tiles[y][x-1].getDiscovered()){
			blankFinder(tiles,x-1,y);
		}
		if (tiles[y][x-1].getType()){
			tiles[y][x-1].discover();
			if(y<height-1){
				if(tiles[y+1][x-1].getType()==BLANK && !tiles[y+1][x-1].getDiscovered())
					blankFinder(tiles,x-1,y+1);
				else
					tiles[y+1][x-1].discover();
			}
		}
				
	}
}


int discoverAdjTile(Tile** tiles, int x, int y){
	/* Allows the double click on discovered numbers to discover all of the adjacent ones */
	if(!hasEnoughFlags(tiles,x,y)) return 0;
	int bomb = 0;
	int temp;
	if(x>0 && y>0){
		if((temp=tiles[y-1][x-1].discover())==2) {//if discover a blank, then blankFinder() it
			blankFinder(tiles,x-1,y-1);
		} else if(temp==1) bomb = 1; //if it's a bomb
	}
	if(x>0){
		if((temp=tiles[y][x-1].discover())==2){
			blankFinder(tiles,x-1,y);
		} else if(temp==1) bomb = 1;
	}
	if(x>0 && y<height-1){
		if((temp=tiles[y+1][x-1].discover())==2){
			blankFinder(tiles,x-1,y+1);
		} else if(temp==1) bomb = 1;
	}

	if(y>0){
		if((temp=tiles[y-1][x].discover())==2){
			blankFinder(tiles,x,y-1);
		} else if(temp==1) bomb = 1;
	}
	if(y<height-1) {
		if((temp=tiles[y+1][x].discover())==2){
			blankFinder(tiles,x,y+1);
		} else if(temp==1) bomb = 1;
	}

	if(x<width-1 && y>0) {
		if((temp=tiles[y-1][x+1].discover())==2){
			blankFinder(tiles,x+1,y-1);
		} else if(temp==1) bomb = 1;
	}
	if(x<width-1) {
		if((temp=tiles[y][x+1].discover())==2){
			blankFinder(tiles,x+1,y);
		} else if(temp==1) bomb = 1;
	}
	if(x<width-1 && y<height-1){
		if((temp=tiles[y+1][x+1].discover())==2){
			blankFinder(tiles,x+1,y+1);
		} else if(temp==1) bomb = 1;
	}
	return bomb;
}


int hasEnoughFlags(Tile** tiles, int x, int y){
	int totFlags = 0;

	if(x>0 && y>0) 				totFlags += tiles[y-1][x-1].getFlaged();
	if(x>0) 					totFlags += tiles[y][x-1].getFlaged();
	if(x>0 && y<height-1) 		totFlags += tiles[y+1][x-1].getFlaged();

	if(y>0) 					totFlags += tiles[y-1][x].getFlaged();
	if(y<height-1) 				totFlags += tiles[y+1][x].getFlaged(); 

	if(x<width-1 && y>0) 		totFlags += tiles[y-1][x+1].getFlaged();
	if(x<width-1) 				totFlags += tiles[y][x+1].getFlaged();
	if(x<width-1 && y<height-1) totFlags += tiles[y+1][x+1].getFlaged();

	if(totFlags == tiles[y][x].getType()) return 1;
	return 0;
}



int checkIfWon(){
	if(flags == bombs && flags+discoveredTiles == width*height)
		return 1;
	return 0;
}