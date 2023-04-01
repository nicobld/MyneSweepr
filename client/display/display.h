#pragma once
#include <SDL2/SDL.h>

#include <chrono>

#include "../state/state.h"

#define WINDOW_WIDTH 50*16
#define WINDOW_HEIGHT 50*16

#define SLEEP_DISPLAY 20*1000

#define ERROR_EXIT_SDL SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", SDL_GetError()); \
	exit(EXIT_FAILURE); \


class State;
class Client;

struct MouseEvent{
	SDL_Point point;
	char clicked;
	Uint8 button;
	Uint8 clicks;
};

class Display {
public:
	bool quit;
	Display(State* state, Client* client);
	void update();
	void render();

private:
	State* state;
	Client* client;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Surface* gameSurface;
	SDL_Texture* gameTexture;
	SDL_Event events;
	struct MouseEvent mouseEvent;

	long int lastRender;
};