#include "game_math.h"
double dist(float ax, float ay, float bx, float by) {
	return sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay));
}
