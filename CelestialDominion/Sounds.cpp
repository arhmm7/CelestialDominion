#include <iostream>

#include <SDL.h>
#include <SDL_ttf.h>
#include  <SDL_mixer.h>

#include "Sounds.h"

struct _Mix_Music* backgroundMusic;

struct Mix_Chunk* projectileSound;
struct Mix_Chunk* explosionSound;
struct Mix_Chunk* reloadSound;
struct Mix_Chunk* errorSound;

bool loadMedia(SDL_Window* window) 
{
	bool success = true;

	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		std::cout << "Failed to initialize SDL Audio : " << SDL_GetError() << std::endl;
	}


	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Failed to open audio devices!", Mix_GetError(), window);
		success = false;
	}

	projectileSound = Mix_LoadWAV("./audios/shoot.wav");
	if (projectileSound == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Failed to load resources!", Mix_GetError(), window);
		success = false;
	}

	explosionSound = Mix_LoadWAV("./audios/explosion.wav");
	if (explosionSound == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Failed to load resources!", Mix_GetError(), window);
		success = false;
	}

	reloadSound = Mix_LoadWAV("./audios/reload.wav");
	if (reloadSound == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Failed to load resources!", Mix_GetError(), window);
		success = false;
	}

	errorSound = Mix_LoadWAV("./audios/error.wav");
	if (errorSound == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Failed to load resources!", Mix_GetError(), window);
		success = false;
	}

	backgroundMusic = Mix_LoadMUS("./audios/background.mp3");
	if (backgroundMusic == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Failed to load resources!", Mix_GetError(), window);
		success = false;
	}

	Mix_Volume(2, 50);

	return success;
}

void close() {
	Mix_FreeChunk(projectileSound);
	Mix_FreeChunk(explosionSound);
	Mix_FreeMusic(backgroundMusic);
	Mix_Quit();
}

