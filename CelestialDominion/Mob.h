#pragma once
#include <iostream>
#include <stdlib.h>

#include "Projectile.h"
#include "Math.h"

class Mob
{
public:
	enum Type
	{
		FURY,
		EARTHLING
	};

	int id;
	int health;
	int speed;
	int damage;
	double angle;
	int x = 0,y = 0,w = 0,h = 0;
	bool active;

	int getDamageById(int id);
	int getHealthById(int id);
	void initMob(int ship_x, int ship_y, int ship_w, int ship_h, int mob_id);
};

