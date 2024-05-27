#include <iostream>
#include <cmath>
#include <string>
#include <vector>

#include <SDL.h>
#include <SDL_ttf.h>

#include "Math.h"
#include "Vector.h"
#include "Projectile.h"
#include "Mob.h"
#include "Player.h"


int main(int argc , char* argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Failed to initialize SDL : " << SDL_GetError() << std::endl;
	}

	SDL_Window* window;
	window = SDL_CreateWindow("Celestial Dominion",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
	if (window==NULL) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Failed to initialize SDL_Window!", SDL_GetError(), window);
	}

	SDL_Renderer* renderer;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Failed to initialize SDL_Renderer!", SDL_GetError(), window);
	}

	SDL_Surface* backgroundSurface = SDL_LoadBMP("./images/background.bmp");
	SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
	SDL_SetTextureAlphaMod(backgroundTexture, 100);
	SDL_FreeSurface(backgroundSurface);

	SDL_Surface* shipSurface = SDL_LoadBMP("./images/ship.bmp");
	SDL_SetColorKey(shipSurface,SDL_TRUE,SDL_MapRGB(shipSurface->format,0,0,0));
	SDL_Texture* shipTexture = SDL_CreateTextureFromSurface(renderer,shipSurface);
	SDL_FreeSurface(shipSurface);

	SDL_Surface* projectileSurface = SDL_LoadBMP("./images/projectile.bmp");
	SDL_SetColorKey(projectileSurface, SDL_TRUE, SDL_MapRGB(projectileSurface->format, 0, 0, 0));
	SDL_Texture* projectileTexture = SDL_CreateTextureFromSurface(renderer, projectileSurface);
	SDL_FreeSurface(projectileSurface);

	SDL_Surface* furySurface = SDL_LoadBMP("./images/fury.bmp");
	SDL_SetColorKey(furySurface, SDL_TRUE, SDL_MapRGB(furySurface->format, 0, 0, 0));
	SDL_Texture* furyTexture = SDL_CreateTextureFromSurface(renderer, furySurface);
	SDL_FreeSurface(furySurface);

	TTF_Init();

	TTF_Font* Font = TTF_OpenFont("./fonts/Poppins-Medium.ttf",40);
	if (Font==NULL) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Failed to load resources!", SDL_GetError(), window);
		SDL_Quit();
	}

	SDL_Color white = {255,255,255};
	SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(Font, "Celestial Dominion",white, 500);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

	SDL_Rect text;
	text.x = (1280 / 2) - (textSurface->w / 2);
	text.y = 50;
	text.w = textSurface->w;
	text.h = textSurface->h;

	SDL_Rect background;
	background.x = 0;
	background.y = 0;
	background.w = 1280;  
	background.h = 720;

	SDL_Rect ship;
	ship.x = (SCREEN_WIDTH/2)-50;
	ship.y = (SCREEN_HEIGHT/2) - 50;
	ship.w = 100;
	ship.h = 100;

	std::vector<Projectile> projectiles;
	std::vector<Mob> mobs;
	Vector mousePos;

	bool appRunnig = true;
	double angle = 0.0;
	double projectileAngle = 0.0;

	Player ply;

	while (appRunnig) {

		if (ply.health <= 0) {
			appRunnig = false;
		}

		static Uint32 lastTime = SDL_GetTicks();
		Uint32 currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - lastTime) / 1000.0f;
		lastTime = currentTime;

		SDL_Event event;
		angle = calculateAngle(ship.x + ship.w / 2, ship.y + ship.h / 2, mousePos.getX(), mousePos.getY());
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
			{
				appRunnig = false;
			}
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					projectileAngle = angle;
					Projectile newProjectile;
					newProjectile.x = ship.x+25;
					newProjectile.y = ship.y+25;
					newProjectile.w = 50;
					newProjectile.h = 50;
					newProjectile.active = true;
					newProjectile.speed = 1000;
					newProjectile.angle = projectileAngle;
					projectiles.push_back(newProjectile);
				}
				else if (event.button.button == SDL_BUTTON_RIGHT) {
					Mob newMob;
					
					newMob.x = getRandomX();
					newMob.y = getRandomY();
					
					
					newMob.id = "FURY";
					newMob.w = 100;
					newMob.h = 100;
					newMob.angle = calculateAngle(ship.x + ship.w / 2, ship.y + ship.h / 2, newMob.x+newMob.w/2, newMob.y+newMob.h/2);
					newMob.speed = 500;
					newMob.active = true;
					mobs.push_back(newMob);
				}
			}
			if (event.type == SDL_MOUSEMOTION)
			{
				mousePos.setPosition(event.motion.x, event.motion.y);
			}
			if (event.type == SDL_KEYDOWN)
			{

			}
		}

		for (auto& projectile:projectiles) {
			if (projectile.active) {

				float velX = projectile.speed * std::cos(projectile.angle) * deltaTime;
				float velY = projectile.speed * std::sin(projectile.angle) * deltaTime;

				projectile.x += static_cast<int>(velX);
				projectile.y += static_cast<int>(velY);

				if (projectile.x < 0 || projectile.x > SCREEN_WIDTH || projectile.y < 0 || projectile.y > SCREEN_HEIGHT) {
					projectile.active = false;
				}
			}
		}

		for (auto& mob : mobs) {
			if (mob.active) {

				float velX = -mob.speed * std::cos(mob.angle) * deltaTime;
				float velY = -mob.speed * std::sin(mob.angle) * deltaTime;

				mob.x += static_cast<int>(velX);
				mob.y += static_cast<int>(velY);

				if (mob.x < ship.x + ship.w &&
					mob.x + mob.w > ship.x &&
					mob.y < ship.y + ship.h &&
					mob.y + mob.h > ship.y) {
					mob.active = false;
					ply.health -= 25;
					std::cout << ply.health << std::endl;
				}

			}
		}

		for (auto& projectile : projectiles) {
			if (projectile.active) {
				for (auto& mob : mobs) {
					if (mob.active) {
						if (mob.x < projectile.x + projectile.w &&
							mob.x + mob.w > projectile.x &&
							mob.y < projectile.y + projectile.h &&
							mob.y + mob.h > projectile.y) {
							mob.active = false;
							projectile.active = false;
							ply.score += 5;
						}
					}
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 0,0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, backgroundTexture, NULL, &background);
		SDL_RenderCopy(renderer, textTexture, NULL, &text);


		SDL_SetRenderDrawColor(renderer, 255, 0, 255, SDL_ALPHA_OPAQUE);
		for (const auto& projectile : projectiles) {
			if (projectile.active) {
				SDL_Rect projectileRect = { static_cast<int>(projectile.x), static_cast<int>(projectile.y), 50, 50 };
				SDL_RenderCopyEx(renderer, projectileTexture, nullptr, &projectileRect, projectile.angle* (180.0 / M_PI) + 90.0, NULL, SDL_FLIP_NONE);
				// SDL_RenderDrawRect(renderer,&projectileRect);
			}
		}

		for (const auto& mob : mobs) {
			if (mob.active) {
				SDL_Rect mobRect = { static_cast<int>(mob.x), static_cast<int>(mob.y), 100, 100 };
				SDL_RenderCopyEx(renderer, furyTexture, nullptr, &mobRect, mob.angle * (180.0 / M_PI)-90, NULL, SDL_FLIP_NONE);
				// SDL_RenderDrawRect(renderer, &mobRect);
			}
		}
		
		SDL_RenderCopyEx(renderer, shipTexture, NULL, &ship, (angle * (180 / M_PI)) + 90.0, NULL, SDL_FLIP_NONE);
		// SDL_RenderDrawRect(renderer, &ship);
		SDL_RenderPresent(renderer);
		ply.score +=(int) deltaTime;
	}
	std::string msg = "Score :" + std::to_string(ply.score);
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION , "Game Over" , msg.c_str(),window );

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(textTexture);
	SDL_DestroyTexture(projectileTexture);
	SDL_DestroyTexture(backgroundTexture);
	SDL_DestroyTexture(shipTexture);
	SDL_DestroyTexture(furyTexture);
	TTF_CloseFont(Font);
	TTF_Quit();
	SDL_Quit();
	return 0;
}