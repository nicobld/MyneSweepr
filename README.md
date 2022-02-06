# MyneSweepr
An epic MineSweeper game made in C++ with the SDL2 library.  

First click on tile might be long because of a feature I added that starts the game only if you click on a blank tile.  
Makes it more convenient to start the game.

## Requirements
_-> SDL2_  
You can install it with Homebrew  
`brew install SDL2`  

## Run
Start by compiling if not compiled  

`make`  

Then to run the game either do  

`./prog "easy/medium"`  

or  

`./prog <width> <height> <bombs>`  

## To Do

- [ ] Timer
- [x] Block doubleclick if numer != flags aroud it
- [x] Add every number from 4 to 8
- [ ] Start Menu
- [ ] End Menu
- [x] Restarting
- [ ] HighScores
- [ ] Multiplayer idk
- [x] Always start by clicking on a blank
