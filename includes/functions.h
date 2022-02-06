#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "tile.h"
#include "common.h"

extern int discoveredTiles;

void initTiles();
void blankFinder(int, int);
int discoverAdjTile(int, int);
int checkIfWon();
int hasEnoughFlags(int,int);

#endif