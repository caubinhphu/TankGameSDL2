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