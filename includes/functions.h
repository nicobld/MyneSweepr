#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "tile.h"

extern int discoveredTiles;

void initTiles(Tile** tiles);
void blankFinder(Tile** tiles, int, int);
int discoverAdjTile(Tile** tiles, int, int);
int checkIfEnded();

#endif