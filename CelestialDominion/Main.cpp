#include <iostream>
#include <cmath>
#include <string>
#include <vector>

#include <SDL.h>
#include <SDL_ttf.h>
#include  <SDL_mixer.h>
#include <random>

#include "Math.h"
#include "Vector.h"
#include "Projectile.h"
#include "Mob.h"
#include "Player.h"
#include "Sounds.h"
#include "Texture.h"


SDL_Window* window;
SDL_Renderer* renderer;

SDL_Color white = { 255,255,255 };
SDL_Color primary = { 0 , 255 , 0 };

int main(int argc , char* argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		std::cout << "Failed to initialize SDL Video : " << SDL_GetError() << std::endl;
	}

	window = SDL_CreateWindow("Celestial Dominion",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
	if (window==NULL) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Failed to initialize SDL_Window!", SDL_GetError(), window);
	}


	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Failed to initialize SDL_Renderer!", SDL_GetError(), window);
	}

	loadMedia(window);

	Texture* textures = new Texture(renderer);

	TTF_Init();

	TTF_Font* Font = TTF_OpenFont("./fonts/Poppins-Medium.ttf",40);
	if (Font==NULL) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Failed to load resources!", SDL_GetError(), window);
		SDL_Quit();
	}

	TTF_Font* mediumFont = TTF_OpenFont("./fonts/Poppins-Medium.ttf", 30);
	if (Font == NULL) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Failed to load resources!", SDL_GetError(), window);
		SDL_Quit();
	}

	TTF_Font* smallFont = TTF_OpenFont("./fonts/Poppins-Medium.ttf", 20);
	if (Font == NULL) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Failed to load resources!", SDL_GetError(), window);
		SDL_Quit();
	}

	
	SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(Font, "Celestial Dominion",white, 500);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);

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


	double angle = 0.0;
	double projectileAngle = 0.0;

	bool reload = false;
	const double RELOAD_TIME = 150000;
	const double SPAWN_TIME = 800000;
	double timeSinceReload = 0;
	int timeSinceSpawn = 0;

	bool startRunning = true;
	bool gameRunning = true;
	bool overRunning = true;

	double rp;
	Player ply;

	Mix_PlayMusic(backgroundMusic,-1);

	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, textures->backgroundTexture, NULL, &background);
	SDL_RenderCopy(renderer, textTexture, NULL, &text);
	SDL_RenderPresent(renderer);

	SDL_Rect startButton;
	startButton.x = (SCREEN_WIDTH / 2) - 180;
	startButton.y = 500;
	startButton.w = 360;
	startButton.h = 100;

	SDL_SetRenderDrawColor(renderer, 33, 31, 38, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &startButton);

	SDL_Surface* startSurface = TTF_RenderText_Blended_Wrapped(mediumFont, "Start", white, 360);
	SDL_Texture* startTexture = SDL_CreateTextureFromSurface(renderer, startSurface);
	SDL_FreeSurface(startSurface);

	SDL_Rect startText;
	startText.x = (SCREEN_WIDTH / 2) - startSurface->w/2;
	startText.y = 550 - startSurface->h/2;
	startText.w = startSurface->w;
	startText.h = startSurface->h;

	SDL_RenderCopy(renderer,startTexture,NULL,&startText);

	SDL_Surface* creditsSurface = TTF_RenderText_Blended_Wrapped(smallFont, "By Arham Khan", white, 360);
	SDL_Texture* creditsTexture = SDL_CreateTextureFromSurface(renderer, creditsSurface);
	SDL_FreeSurface(creditsSurface);

	SDL_Rect creditsText;
	creditsText.x = (SCREEN_WIDTH / 2) - creditsSurface->w / 2;
	creditsText.y = 105;
	creditsText.w = creditsSurface->w;
	creditsText.h = creditsSurface->h;

	SDL_RenderCopy(renderer, creditsTexture, NULL, &creditsText);


	SDL_RenderPresent(renderer);

	while(startRunning){
		SDL_Event event;
		while (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT)
			{
				startRunning = false;
				gameRunning = false;
			}

			if (event.type == SDL_MOUSEMOTION)
			{
				mousePos.setPosition(event.motion.x, event.motion.y);
			}
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					if (startButton.x < mousePos.getX() &&
						startButton.x + startButton.w > mousePos.getX() &&
						startButton.y < mousePos.getY() &&
						startButton.y + startButton.h > mousePos.getY()) {
						startRunning = false;
					}
				}
			}
		}
	}
	
	while (gameRunning) 
	{
		static Uint32 lastTime = SDL_GetTicks();
		Uint32 currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - lastTime) / 1000.0f;
		lastTime = currentTime;

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, textures->backgroundTexture, NULL, &background);


		if (reload) 
		{
			timeSinceReload += (double)SDL_GetTicks();
			double rp = (timeSinceReload / RELOAD_TIME) * 100.0;
			std::string rel = "Reloading : " + std::to_string((int)rp) + "%";

			SDL_Surface* reloadSurface = TTF_RenderText_Blended_Wrapped(smallFont, rel.c_str(), white, 300);
			SDL_Texture* reloadTexture = SDL_CreateTextureFromSurface(renderer, reloadSurface);
			SDL_FreeSurface(reloadSurface);

			SDL_Rect reloadRect;
			reloadRect.x = (1280 / 2) - (reloadSurface->w / 2);
			reloadRect.y = ship.y + ship.h + 20;
			reloadRect.w = reloadSurface->w;
			reloadRect.h = reloadSurface->h;

			SDL_RenderCopy(renderer, reloadTexture, NULL, &reloadRect);
		}

		if (timeSinceReload >= RELOAD_TIME) 
		{
			timeSinceReload = 0.0;
			reload = false;
			ply.ammo += 10;
		}

		timeSinceSpawn += SDL_GetTicks();

		if (timeSinceSpawn >= SPAWN_TIME) 
		{
			timeSinceSpawn = 0;
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(0, 1);
			Mob mob;
			mob.initMob(ship.x, ship.y, ship.w, ship.y,dis(gen));
			mobs.push_back(mob);
		}

		if (ply.health <= 0) { gameRunning = false; }

		SDL_Event event;
		angle = calculateAngle(ship.x + ship.w / 2, ship.y + ship.h / 2, mousePos.getX(), mousePos.getY());
		while (SDL_PollEvent(&event)) 
		{
			if (event.type == SDL_QUIT)
			{
				gameRunning = false;
			}
			if (event.type == SDL_MOUSEBUTTONDOWN) 
			{
				if (event.button.button == SDL_BUTTON_LEFT) 
				{
					if (ply.ammo > 0 && !reload) 
					{
						projectileAngle = angle;
						Projectile newProjectile;
						newProjectile.x = ship.x + 25;
						newProjectile.y = ship.y + 25;
						newProjectile.w = 50;
						newProjectile.h = 50;
						newProjectile.active = true;
						newProjectile.damage = 100;
						newProjectile.speed = 1000;
						newProjectile.angle = projectileAngle;
						projectiles.push_back(newProjectile);
						Mix_PlayChannel(-1, projectileSound, 0);
						ply.ammo -= 1;
					}
					else 
					{
						Mix_PlayChannel(-1, errorSound, 0);
					}
					
				}
				else if (event.button.button == SDL_BUTTON_RIGHT) 
				{
					if (!reload) 
					{
						Mix_PlayChannel(-1, reloadSound, 0);
						reload = true;
					}
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

		for (auto& projectile:projectiles) 
		{
			if (projectile.active) 
			{

				float velX = projectile.speed * std::cos(projectile.angle) * deltaTime;
				float velY = projectile.speed * std::sin(projectile.angle) * deltaTime;

				projectile.x += static_cast<int>(velX);
				projectile.y += static_cast<int>(velY);

				if (projectile.x < 0 || projectile.x > SCREEN_WIDTH || projectile.y < 0 || projectile.y > SCREEN_HEIGHT) 
				{
					projectile.active = false;
				}
			}
		}


		for (auto& projectile : projectiles) 
		{
			if (projectile.active) 
			{
				for (auto& mob : mobs) 
				{
					if (mob.active) 
					{
						if (mob.x < projectile.x + projectile.w &&
							mob.x + mob.w > projectile.x &&
							mob.y < projectile.y + projectile.h &&
							mob.y + mob.h > projectile.y) 
						{
							if (mob.health - projectile.damage <= 0)
							{
								mob.active = false;
								Mix_PlayChannel(2, explosionSound, 0);
							}
							mob.health -= projectile.damage;
							projectile.active = false;
							ply.score += 5;
							if (ply.health < 100) {
								ply.health += 2;
							}
						}
					}
				}
			}
		}


		for (auto& mob : mobs) 
		{
			if (mob.active) 
			{

				float velX = -mob.speed * std::cos(mob.angle) * deltaTime;
				float velY = -mob.speed * std::sin(mob.angle) * deltaTime;

				mob.x += static_cast<int>(velX);
				mob.y += static_cast<int>(velY);

				if (mob.x < ship.x + ship.w &&
					mob.x + mob.w > ship.x &&
					mob.y < ship.y + ship.h &&
					mob.y + mob.h > ship.y) 
				{
					mob.active = false;
					Mix_PlayChannel(2, explosionSound, 0);					
					ply.health -= mob.damage;
				}
			}
		}

		
		SDL_RenderCopy(renderer, textTexture, NULL, &text);

		std::string rel = "Score : " + std::to_string(ply.score);
		SDL_Surface* scoreSurface = TTF_RenderText_Blended_Wrapped(smallFont, rel.c_str(), white, 300);
		SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
		SDL_FreeSurface(scoreSurface);

		SDL_Rect scoreRect;
		scoreRect.x = (1280 / 2) - (scoreSurface->w / 2);
		scoreRect.y = ship.y - 35;
		scoreRect.w = scoreSurface->w;
		scoreRect.h = scoreSurface->h;

		SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);

		std::string hel = "Health : " + std::to_string(ply.health);
		SDL_Surface* healthSurface = TTF_RenderText_Blended_Wrapped(smallFont, hel.c_str(), white, 300);
		SDL_Texture* healthTexture = SDL_CreateTextureFromSurface(renderer, healthSurface);
		SDL_FreeSurface(healthSurface);

		SDL_Rect healthRect;
		healthRect.x = (1280 / 2) - (healthSurface->w / 2);
		healthRect.y = ship.y - 70;
		healthRect.w = healthSurface->w;
		healthRect.h = healthSurface->h;

		SDL_RenderCopy(renderer, healthTexture, NULL, &healthRect);

		SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
		for (const auto& projectile : projectiles) 
		{
			if (projectile.active) 
			{
				SDL_Rect projectileRect = { static_cast<int>(projectile.x), static_cast<int>(projectile.y), 50, 50 };
				SDL_RenderCopyEx(renderer, textures->projectileTexture, nullptr, &projectileRect, projectile.angle* (180.0 / M_PI) + 90.0, NULL, SDL_FLIP_NONE);
				// SDL_RenderDrawRect(renderer,&projectileRect);
			}
		}


		SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		for (const auto& mob : mobs) 
		{
			if (mob.active) 
			{
				SDL_Rect mobRect = { static_cast<int>(mob.x), static_cast<int>(mob.y), 100, 100 };
				SDL_RenderCopyEx(renderer, textures->getTextureById(mob.id), nullptr, &mobRect, mob.angle* (180.0 / M_PI) - 90, NULL, SDL_FLIP_NONE);
				// SDL_RenderDrawRect(renderer, &mobRect);
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderCopyEx(renderer, textures->shipTexture, NULL, &ship, (angle * (180 / M_PI)) + 90.0, NULL, SDL_FLIP_NONE);
		// SDL_RenderDrawRect(renderer, &ship);

		SDL_RenderPresent(renderer);

		if (Mix_PlayingMusic() == 0)
		{
			Mix_PlayMusic(backgroundMusic, -1);
		}
		SDL_Delay(16);
	}

	SDL_RenderCopy(renderer, textures->backgroundTexture, NULL, &background);
	SDL_RenderCopy(renderer, textTexture, NULL, &text);

	SDL_Rect overButton;
	overButton.x = (SCREEN_WIDTH / 2) - 180;
	overButton.y = 500;
	overButton.w = 360;
	overButton.h = 100;

	SDL_Surface* overSurface = TTF_RenderText_Blended_Wrapped(mediumFont, "Exit", white, 360);
	SDL_Texture* overTexture = SDL_CreateTextureFromSurface(renderer, overSurface);
	SDL_FreeSurface(overSurface);


	SDL_Rect overText;
	overText.x = (SCREEN_WIDTH / 2) - overSurface->w / 2;
	overText.y = 550 - overSurface->h / 2;
	overText.w = overSurface->w;
	overText.h = overSurface->h;

	SDL_SetRenderDrawColor(renderer, 33, 31, 38, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &overButton);
	SDL_RenderCopy(renderer, overTexture, NULL, &overText);

	std::string scr = "Score : " + std::to_string(ply.score);

	SDL_Surface* scoreSurface = TTF_RenderText_Blended_Wrapped(Font, scr.c_str(), white, 360);
	SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
	SDL_FreeSurface(scoreSurface);


	SDL_Rect scoreText;
	scoreText.x = (SCREEN_WIDTH / 2) - scoreSurface->w / 2;
	scoreText.y = (SCREEN_HEIGHT / 2) - scoreSurface->h / 2;
	scoreText.w = scoreSurface->w;
	scoreText.h = scoreSurface->h;

	
	SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreText);
	SDL_RenderPresent(renderer);



	while (overRunning) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT)
			{
				overRunning = false;
			}

			if (event.type == SDL_MOUSEMOTION)
			{
				mousePos.setPosition(event.motion.x, event.motion.y);
			}
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					if (overButton.x < mousePos.getX() &&
						overButton.x + overButton.w > mousePos.getX() &&
						overButton.y < mousePos.getY() &&
						overButton.y + overButton.h > mousePos.getY()) {
						overRunning = false;
					}
				}
			}
		}
	}

	Mix_HaltMusic();

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(textTexture);
	close();
	TTF_CloseFont(Font);
	TTF_Quit();
	SDL_Quit();
	return 0;
}