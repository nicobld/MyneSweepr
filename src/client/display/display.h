#pragma once
#include <SDL2/SDL.h>
#include "../state/state.h"

#define WINDOW_WIDTH 50*16
#define WINDOW_HEIGHT 50*16

#define ERROR_EXIT_SDL SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", SDL_GetError()); \
	exit(EXIT_FAILURE); \


class State;


struct MouseEvent{
	SDL_Point point;
	char clicked;
	Uint8 button;
	Uint8 clicks;
};

class Display {
public:
	bool quit;
	Display();
	void update();
	void render();

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Surface* gameSurface;
	SDL_Texture* gameTexture;
	SDL_Event events;
	struct MouseEvent mouseEvent;
};