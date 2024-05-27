#include "Math.h"
#include <cmath>


double calculateAngle(int x1, int y1, int x2, int y2) {
	return atan2(y2 - y1, x2 - x1);
}