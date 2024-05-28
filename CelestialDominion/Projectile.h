#pragma once
enum Dimensions
{
	SCREEN_WIDTH = 1280,
	SCREEN_HEIGHT = 720
};

struct Projectile
{
	int w;
	int h;
	int x;
	int y;
	int damage;
	int speed;
	double angle;
	bool active;
};



