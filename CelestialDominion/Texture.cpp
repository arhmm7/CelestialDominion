#include "Texture.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include  <SDL_mixer.h>

Texture::Texture(SDL_Renderer* renderer) 
{
	backgroundSurface = SDL_LoadBMP("./images/background.bmp");
	backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
	SDL_SetTextureAlphaMod(backgroundTexture, 100);
	SDL_FreeSurface(backgroundSurface);

	 shipSurface = SDL_LoadBMP("./images/ship.bmp");
	SDL_SetColorKey(shipSurface, SDL_TRUE, SDL_MapRGB(shipSurface->format, 0, 0, 0));
	shipTexture = SDL_CreateTextureFromSurface(renderer, shipSurface);
	SDL_FreeSurface(shipSurface);

	projectileSurface = SDL_LoadBMP("./images/projectile.bmp");
	SDL_SetColorKey(projectileSurface, SDL_TRUE, SDL_MapRGB(projectileSurface->format, 0, 0, 0));
	projectileTexture = SDL_CreateTextureFromSurface(renderer, projectileSurface);
	SDL_FreeSurface(projectileSurface);

	furySurface = SDL_LoadBMP("./images/fury.bmp");
	SDL_SetColorKey(furySurface, SDL_TRUE, SDL_MapRGB(furySurface->format, 0, 0, 0));
	furyTexture = SDL_CreateTextureFromSurface(renderer, furySurface);
	SDL_FreeSurface(furySurface);

	earthlingSurface = SDL_LoadBMP("./images/earthling.bmp");
	SDL_SetColorKey(earthlingSurface, SDL_TRUE, SDL_MapRGB(earthlingSurface->format, 0, 0, 0));
	earthlingTexture = SDL_CreateTextureFromSurface(renderer, earthlingSurface);
	SDL_FreeSurface(earthlingSurface);
}

Texture::~Texture(){

	SDL_DestroyTexture(projectileTexture);
	SDL_DestroyTexture(backgroundTexture);
	SDL_DestroyTexture(shipTexture);
	SDL_DestroyTexture(furyTexture);

}

SDL_Texture* Texture::getTextureById(int id) {
	switch (id)
	{
	case 0:
		return furyTexture;
		break;
	case 1:
		return earthlingTexture;
		break;
	default:
		return NULL;
		break;
	}
}