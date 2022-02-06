#ifndef MAIN_H
#define MAIN_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <sys/types.h>

#define ERROR_EXIT SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"%s",SDL_GetError());SDL_Quit(); return EXIT_FAILURE;


#endif