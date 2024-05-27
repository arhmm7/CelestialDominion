#pragma once
#include <iostream>

#include "Projectile.h"
#include <stdlib.h>

class Mob
{
	int lbx = 1280, ubx = 1700;
	int lby = 720, uby = 1200;

public:
	

	Mob() {
		std::cout << "Mob Spawned" << std::endl;
	}
	~Mob() {
		std::cout << "Mob Deleted" << std::endl;
	}


};

