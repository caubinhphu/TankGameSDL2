#include "Bullet.h"

Bullet::Bullet() {
	spX = spY = 0;
	rotation = 0;
	isMove = false;
}

Bullet::~Bullet() {
	;
}

void Bullet::move() {
	if (dir == TOP_LEFT) {
		box.x -= spX;
		if (box.x <= 50) {
			isMove = false;
		}
		box.y -= spY;
		if (box.y <= 50) {
			isMove = false;
		}
	}
	else if (dir == TOP_RIGHT) {
		box.x += spX;
		if (box.x >= backgroundWidth) {
			isMove = false;
		}
		box.y -= spY;
		if (box.y <= 0) {
			isMove = false;
		}
	}
	else if (dir == BOTTOM_LEFT) {
		box.x -= spX;
		if (box.x <= 0) {
			isMove = false;
		}
		box.y += spY;
		if (box.y >= backgroundHeight) {
			isMove = false;
		}
	}
	else if (dir == BOTTOM_RIGHT) {
		box.x += spX;
		if (box.x >= backgroundWidth) {
			isMove = false;
		}
		box.y += spY;
		if (box.y >= backgroundHeight) {
			isMove = false;
		}
	}
}

void Bullet::render(SDL_Renderer* _renderer, SDL_Rect _camera) {
	BasicObj::render(_renderer, box.x - _camera.x, box.y - _camera.y, NULL, rotation);
}