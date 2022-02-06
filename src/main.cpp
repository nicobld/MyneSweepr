#include "../includes/main.h"
#include "../includes/tile.h"
#include "../includes/common.h"
#include "../includes/functions.h"


int width = 10;
int height = 10;
int bombs = 20;
int flags = 0;
int discoveredTiles = 0;


struct MouseEvent{
	SDL_Point point;
	char clicked;
	Uint8 button;
	Uint8 clicks;
};


int main(int argc,char* argv[]){
	if(argc==4){
		width = atoi(argv[1]);
		height = atoi(argv[2]);
		bombs = atoi(argv[3]);
	}

	if (argc==2){
		if(!strncmp(argv[1],"easy",4)){
			width = 10;
			height = 10;
			bombs = 10;
		}else if (!strncmp(argv[1],"medium",6)){
			width = 30;
			height = 30;
			bombs = 150;
		}
	}


	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)<0){
		ERROR_EXIT
	}

	SDL_Window* window;
	SDL_Renderer* renderer;
	if(SDL_CreateWindowAndRenderer(WINDOW_WIDTH,WINDOW_HEIGHT,SDL_WINDOW_SHOWN,&window,&renderer)==-1){
		ERROR_EXIT
	}
	SDL_SetWindowTitle(window,"MyneSweepr");

	SDL_Surface* gameSurface;
	if((gameSurface=IMG_Load("img/textures.png"))==NULL){
		ERROR_EXIT
	}
	SDL_Texture* gameTexture;
	if((gameTexture=SDL_CreateTextureFromSurface(renderer,gameSurface))==NULL){
		ERROR_EXIT
	}SDL_FreeSurface(gameSurface);




	//tile creation
	Tile **tiles = (Tile **)malloc(sizeof (Tile* ) * height);
	for(int i=0;i<height;i++)
		tiles[i] = (Tile *)malloc(sizeof (Tile) * width);
	initTiles(tiles);


	SDL_Event events;
	struct MouseEvent mouseEvent;
	mouseEvent.clicked = 0;
	int run = 1;

	while(run && !checkIfEnded()){
		/* EVENT CHECK */
		while(SDL_PollEvent(&events)){
			switch(events.type){
				case SDL_WINDOWEVENT:
					if(events.window.event == SDL_WINDOWEVENT_CLOSE)
						run = 0;
					break;
				case SDL_MOUSEBUTTONDOWN:
					mouseEvent.point.x = events.button.x;
					mouseEvent.point.y = events.button.y;
					mouseEvent.button = events.button.button;
					mouseEvent.clicked = 1;
					mouseEvent.clicks = events.button.clicks;
					break;
				}
			}


		/* RENDERING AND OTHER */
		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_RenderClear(renderer);

		for (int j=0;j<height;j++){
			for (int i=0;i<width;i++){
				if(mouseEvent.clicked){
					SDL_Rect temp = tiles[j][i].getTileRect();
					if(SDL_PointInRect(&mouseEvent.point,&temp)==SDL_TRUE){ //if clicked on the tile
						if(mouseEvent.button == SDL_BUTTON_LEFT){ 			//if left clicked on tile
							if(mouseEvent.clicks == 2){
								if (discoverAdjTile(tiles,i,j) == 1){
									//run = 0;
								}
							}else {
								int discoverValue = tiles[j][i].discover(); 	//discover it
								if (discoverValue == 2) 						//if discovered blank
									blankFinder(tiles,i,j);						//blankFinder algorithm to discover adjacent blanks
								else if (discoverValue == 1) 					//if discovered bomb
									run = 0;
							}
						}
						else if(mouseEvent.button == SDL_BUTTON_RIGHT) 		//if right clicked on tile
							tiles[j][i].flag();								//flag it
					}
				}
				//Rendering
				SDL_Rect src = tiles[j][i].getTileImg();
				SDL_Rect dst = tiles[j][i].getTileRect();
				SDL_RenderCopy(renderer,gameTexture,&src,&dst);
			}
		}

		SDL_RenderPresent(renderer);
		mouseEvent.clicked = 0;
	}

	if(checkIfEnded())
		printf("You won !\n");
	else
		printf("You lost :(\n");

	for(int i=0;i<height;i++)
		free(tiles[i]);
	free(tiles);

	SDL_DestroyTexture(gameTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}