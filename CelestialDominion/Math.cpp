#include "Math.h"

#include <iostream>
#include <random>

double calculateAngle(int x1, int y1, int x2, int y2) {
	return atan2(y2 - y1, x2 - x1);
}

int getRandomX() {
    std::random_device rd1;
    std::random_device rd2;
    std::mt19937 gen1(rd1());
    std::mt19937 gen2(rd2());
    std::uniform_int_distribution<> dis1(1, 2);
    std::uniform_int_distribution<> dis2(0, 400);

    int dir = dis1(gen1);
    int xOffset = dis2(gen2);
    int x_pos = 0;
    switch (dir)
    {
    case 1:
        x_pos = 1280 + xOffset;
        break;
    case 2:
        x_pos = 0 - xOffset;
        break;
    default:
        break;
    }
    return x_pos;
}

int getRandomY() {
    std::random_device rd1;
    std::random_device rd2;
    std::mt19937 gen1(rd1());
    std::mt19937 gen2(rd2());
    std::uniform_int_distribution<> dis1(1, 2);
    std::uniform_int_distribution<> dis2(0, 400);

    int dir = dis1(gen1);
    int yOffset = dis2(gen2);
    int y_pos = 0;
    switch (dir)
    {
    case 1:
        y_pos = 720 + yOffset;
        break;
    case 2:
        y_pos = 0 - yOffset;
        break;
    default:
        break;
    }
    return y_pos;
}