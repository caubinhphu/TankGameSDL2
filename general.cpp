#include "general.h"

double check::rotationA_B(int xA, int yA, int xB, int yB) {
	if (xA >= xB && yA <= yB)
		return (PI / 2 - (atan((double)abs(yA - yB) / abs(xA - xB))));
	else if (xA > xB && yA >= yB)
		return (PI / 2 + (atan((double)abs(yA - yB) / abs(xA - xB))));
	else if (xA <= xB && yA > yB)
		return (1.5 * PI - (atan((double)abs(yA - yB) / abs(xA - xB))));
	else if (xA < xB && yA <= yB)
		return (1.5 * PI + (atan((double)abs(yA - yB) / abs(xA - xB))));
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

int check::binhPhuongKhoangCach(int x1, int y1, int x2, int y2) {
	int dX = x2 - x1;
	int dY = y2 - y1;
	return dX * dX + dY * dY;
}

bool check::checkRect_Circle(SDL_Rect b, Circle a) {
	int x, y;
	if (a.x < b.x) x = b.x;
	else if (a.x > b.x + b.w) x = b.x + b.w;
	else x = a.x;

	if (a.y < b.y) y = b.y;
	else if (a.y > b.y + b.h) y = b.y + b.h;
	else y = a.y;

	int kc_mu2 = check::binhPhuongKhoangCach(a.x, a.y, x, y);
	if (kc_mu2 < a.r * a.r) return true;
	return false;
}

bool check::checkCircle_Circle(Circle a, Circle b)
{
	int tongBanKinh = a.r + b.r;
	int tongBanKinhBinhPhuong = tongBanKinh * tongBanKinh;
	if (check::binhPhuongKhoangCach(a.x, a.y, b.x, b.y) < tongBanKinhBinhPhuong)
		return true;
	return false;
}

bool check::checkInsideRect(int _x, int _y, SDL_Rect a) {
	if (_x < a.x || _x > a.x + a.w || _y < a.y || _y > a.y + a.h)
		return false;
	return true;
}
