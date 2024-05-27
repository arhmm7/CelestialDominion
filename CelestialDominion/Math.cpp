#include "Math.h"
#include <cmath>

double calculateAngle(int x1, int y1, int x2, int y2) {
	return atan2(y2 - y1, x2 - x1);
}

int getRandomX() {
    int x = std::rand() % 1580 - 300 ;
    return x;
}


int getRandomY() {
    int y = std::rand() % 1020 - 300;
    return y;
}
