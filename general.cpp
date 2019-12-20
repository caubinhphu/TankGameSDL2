#include "general.h"

double check::rotationA_B(int xA, int yA, int xB, int yB) {
	if (xA >= xB && yA <= yB)
		return (90 - (180 * atan((double)abs(yA - yB) / abs(xA - xB)) / 3.14));
	else if (xA > xB&& yA >= yB)
		return (90 + (180 * atan((double)abs(yA - yB) / abs(xA - xB)) / 3.14));
	else if (xA <= xB && yA > yB)
		return (180 + (90 - (180 * atan((double)abs(yA - yB) / abs(xA - xB)) / 3.14)));
	else if (xA < xB && yA <= yB) return (270 + (180 * atan((double)abs(yA - yB) / abs(xA - xB)) / 3.14));
}

bool check::checkRect_Rect(SDL_Rect a, SDL_Rect b) {
	int top_a, top_b, bottom_a, bottom_b, left_a, left_b, right_a, right_b;
	top_a = a.y;
	bottom_a = a.y + a.h;
	left_a = a.x;
	right_a = a.x + a.w;

	top_b = b.y;
	bottom_b = b.y + b.h;
	left_b = b.x;
	right_b = b.x + b.w;

	if (bottom_a <= top_b || bottom_b <= top_a || right_a <= left_b || right_b <= left_a) return false;
	return true;
}