#include "Status.h"

void Submarine::Move(int dx, int dy) {
	pos.x += dx;
	pos.y += dy;
	battery -= dx + dy;
}