#include "Bullet.h"

Bullet::Bullet() {
	spX = spY = 0;
	rotation = 0;
	isMove = false;
	isEffectShoot = true;
	type = nomalBullet;
	firtingRate = nomalRate;
	damge = nomalDamge;
}

Bullet::~Bullet() {
	BasicObj::free();
	shootEffect.free();
	collisionEffect.free();
}

void Bullet::move() {
	if (dir == TOP_LEFT) {
		box.x -= spX;
		box.y -= spY;
	}
	else if (dir == TOP_RIGHT) {
		box.x += spX;
		box.y -= spY;
	}
	else if (dir == BOTTOM_LEFT) {
		box.x -= spX;
		box.y += spY;
	}
	else if (dir == BOTTOM_RIGHT) {
		box.x += spX;
		box.y += spY;
	}
}

void Bullet::render(SDL_Renderer* _renderer, SDL_Rect _camera) {
	BasicObj::render(_renderer, box.x - _camera.x, box.y - _camera.y, NULL, rotation);
}

bool Bullet::loadImg(std::string _pathBullet, std::string _pathShootEffect, std::string _pathCollisionEffect, SDL_Renderer* _renderer) {
	if (!BasicObj::loadImg(_pathBullet, _renderer)) {
		return false;
	}
	if (!shootEffect.loadImg(_pathShootEffect, _renderer)) {
		return false;
	}
	if (!collisionEffect.loadImg(_pathCollisionEffect, _renderer)) {
		return false;
	}
	return true;
}

void Bullet::renderShootEffect(SDL_Renderer* _renderer, SDL_Rect _camera, SDL_Rect _boxTankMain) {
	int _x = 0, _y = 0;
	_x = _boxTankMain.x + _boxTankMain.w / 2 + (sin(rotation) * ((_boxTankMain.h / 2) + 6)) - shootEffect.getW() / 2;
	_y = _boxTankMain.y + _boxTankMain.h / 2 - (cos(rotation) * ((_boxTankMain.h / 2) + 6)) - shootEffect.getH() / 2;
	
	shootEffect.render(_renderer, _x - _camera.x, _y - _camera.y, NULL, rotation);
}

void Bullet::renderCollisionEffect(SDL_Renderer* _renderer, SDL_Rect _camera) {
	collisionEffect.render(_renderer, box.x + box.w / 2 - collisionEffect.getW() / 2 - _camera.x, box.y + box.h / 2 - collisionEffect.getH() / 2 - _camera.y, NULL, 0);
}