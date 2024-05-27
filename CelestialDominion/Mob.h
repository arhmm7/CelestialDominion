#pragma once
#include <iostream>
#include <stdlib.h>

#include "Projectile.h"

class Mob
{
public:
	const char* id;
	int health;
	int speed;
	double angle;
	int x = 0,y = 0,w = 0,h = 0;
	bool active;
};

