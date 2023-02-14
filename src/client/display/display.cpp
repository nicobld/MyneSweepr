#include <SDL2/SDL.h>
#include <time.h>
#include <unistd.h>

#include "display.h"
#include "../client/client.h"
#include "../state/state.h"

class State;

extern State state;

Display::Display(){
	mouseEvent.clicked = 0;
	quit = false;

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0){
		ERROR_EXIT_SDL;
	}

	if(SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer) == -1){
		ERROR_EXIT_SDL;
	}
	SDL_SetWindowTitle(window, "MyneSweepr");

	if((gameSurface = SDL_LoadBMP("img/textures.bmp")) == NULL){
		ERROR_EXIT_SDL;
	}

	if((gameTexture = SDL_CreateTextureFromSurface(renderer, gameSurface)) == NULL){
		ERROR_EXIT_SDL;
	} 
	SDL_FreeSurface(gameSurface);
}

void Display::update() {
	char command[BUFSIZE];
	clock_t time_start;
	bool started_clock = false;
	const int trigger = 10; //ms
	SDL_Rect src, dst, temp;
	int i, j;

	while (!quit) {
		if (!started_clock){
			started_clock = true;
			time_start = clock();
		}
		else {
			if (((clock() - time_start) * 1000 / CLOCKS_PER_SEC) >= trigger){
				started_clock = false;
				state.updateState();
			}
		}

		while (SDL_PollEvent(&events)){
			switch (events.type){
				case SDL_WINDOWEVENT:
					if (events.window.event == SDL_WINDOWEVENT_CLOSE)
						quit = true;
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
		if (SDL_RenderClear(renderer) == -1){
			SDL_Log("render clear");
			exit(EXIT_FAILURE);
		}

		if (mouseEvent.clicked){
			for (j = 0; j < state.height; j++){
				for (i = 0; i < state.width; i++){
					temp = state.tiles[j*state.height + i].tileRect;
					if (SDL_PointInRect(&mouseEvent.point, &temp) == SDL_TRUE){ // if clicked on the tile
						int click;
						if (mouseEvent.button == SDL_BUTTON_LEFT)
							click = 0;
						else if (mouseEvent.button == SDL_BUTTON_RIGHT)
							click = 2;
						else
							goto exitfor0;
						if (mouseEvent.clicks == 2)
							click++;
						sprintf(command, "click-%u-%u-%u", i, j, click);
						http_post(command);
					}
				}
			}
		}
		exitfor0:

		for (j = 0; j < state.height; j++){
			for (i = 0; i < state.width; i++){
				// Rendering
				src = state.tiles[j*state.height + i].tileImg;
				dst = state.tiles[j*state.height + i].tileRect;
				if (SDL_RenderCopy(renderer, gameTexture, &src, &dst) < 0){
					SDL_Log("error sdl_rendercopy\n");
					exit(EXIT_FAILURE);
				}
			}
		}

		SDL_RenderPresent(renderer);
		mouseEvent.clicked = 0;
	}

	SDL_DestroyTexture(gameTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}