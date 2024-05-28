#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include  <SDL_mixer.h>

class Texture 
{
public:
	SDL_Surface* backgroundSurface;
	SDL_Texture* backgroundTexture;
	SDL_Surface* shipSurface;
	SDL_Texture* shipTexture;
	SDL_Surface* projectileSurface;
	SDL_Texture* projectileTexture;
	SDL_Surface* furySurface;
	SDL_Texture* furyTexture;
	SDL_Surface* earthlingSurface;
	SDL_Texture* earthlingTexture;

	
	Texture(SDL_Renderer* renderer);
	~Texture();

	SDL_Texture* getTextureById(int id);

};


