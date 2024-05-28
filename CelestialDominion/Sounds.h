#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include  <SDL_mixer.h>

extern Mix_Music* backgroundMusic;

extern Mix_Chunk* projectileSound;
extern Mix_Chunk* explosionSound ;
extern Mix_Chunk* reloadSound;
extern Mix_Chunk* errorSound;


bool loadMedia(SDL_Window* window);
void close();
