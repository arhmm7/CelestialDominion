#include "Mob.h"

#include <iostream>
#include <stdlib.h>

#include "Projectile.h"
#include "Math.h"

int Mob::getDamageById(int id) {
	switch (id)
	{
	case 0:
		return 5;
		break;
	case 1:
		return 5;
		break;
	default:
		return NULL;
		break;
	}
}

int Mob::getHealthById(int id) {
	switch (id)
	{
	case 0:
		return 100;
		break;
	case 1:
		return 100;
		break;
	default:
		return NULL;
		break;
	}
}

void Mob::initMob(int ship_x, int ship_y, int ship_w, int ship_h, int mob_id) {
	x = getRandomX();
	y = getRandomY();
	id = mob_id;
	w = 100;
	h = 100;
	damage = getDamageById(id);
	health = getHealthById(id);
	angle = calculateAngle(ship_x + ship_w / 2, ship_y + ship_h / 2, x + w / 2, y + h / 2);
	speed = 350;
	active = true;
}