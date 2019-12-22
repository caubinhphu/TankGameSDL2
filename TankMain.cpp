#include "TankMain.h"

TankMain::TankMain(int _x, int _y) {
	box.x = _x;
	box.y = _y;
	speed = 5;
	spX = spY = 0;
	rotation = 0;
	//setTankCircle();
	isMouseDown = isMouseUp = false;
}

TankMain::~TankMain() {
	free();
}

void TankMain::setTankCircle() {
	tankCircle = { box.x + box.w / 2, box.y + box.h / 2, box.w / 2 };
}

void TankMain::handleEvents(SDL_Event* _e, SDL_Rect _camera) {
	int mouseX = 0, mouseY = 0;
	SDL_GetMouseState(&mouseX, &mouseY);

	if (mouseX == tankCircle.x && mouseY == tankCircle.y) {
		rotation = 0;
		//std::cout << 1 << std::endl;
	}
	else {
		rotation = check::rotationA_B(mouseX, mouseY, box.x + box.w / 2 - _camera.x, box.y + box.h / 2 - _camera.y);
	}
	if (rotation < 0) rotation = 0;
	//std::cout << rotation << std::endl;

	if (_e->type == SDL_KEYDOWN && _e->key.repeat == 0) {
		switch (_e->key.keysym.sym) {
		case SDLK_a:
			spX -= speed; break;
		case SDLK_d:
			spX += speed; break;
		case SDLK_s:
			spY += speed; break;
		case SDLK_w:
			spY -= speed; break;
		}
	}
	else if (_e->type == SDL_KEYUP && _e->key.repeat == 0) {
		switch (_e->key.keysym.sym) {
		case SDLK_a:
			spX += speed; break;
		case SDLK_d:
			spX -= speed; break;
		case SDLK_s:
			spY -= speed; break;
		case SDLK_w:
			spY += speed; break;
		}
	}
	else if (_e->type == SDL_MOUSEBUTTONDOWN) {
		isMouseDown = true;
		isMouseUp = false;
	}
	else if (_e->type == SDL_MOUSEBUTTONUP) {
		isMouseDown = false;
		isMouseUp = true;
	}
}

void TankMain::move(MapGame map) {
	box.x += spX;
	box.y += spY;
	setTankCircle();
	if (map.checkCollision(tankCircle)) {
		box.x -= spX;
		setTankCircle();
	}
	if (map.checkCollision(tankCircle)) {
		box.y -= spY;
		setTankCircle();
	}
}

void TankMain::setCamera(SDL_Rect &_camera) {
	_camera.x = (box.x + box.w / 2) - cameraWidth / 2;
	_camera.y = (box.y + box.h / 2) - cameraHeight / 2;

	if (_camera.x < 0) {
		_camera.x = 0;
	}
	if (_camera.x > backgroundWidth - cameraWidth) {
		_camera.x = backgroundWidth - cameraWidth;
	}
	if (_camera.y < 0) {
		_camera.y = 0;
	}
	if (_camera.y > backgroundHeight - cameraHeight) {
		_camera.y = backgroundHeight - cameraHeight;
	}
}

void TankMain::render(SDL_Renderer* _renderer, SDL_Rect _camera) {
	BasicObj::render(_renderer, box.x - _camera.x, box.y - _camera.y, NULL, rotation);
}

bool TankMain::loadTamBan(std::string _path, SDL_Renderer* _renderer) {
	return tamBan.loadImg(_path, _renderer);
}

void TankMain::renderTam(SDL_Renderer* _renderer) {
	int mouseX = 0, mouseY = 0;
	SDL_GetMouseState(&mouseX, &mouseY);
	tamBan.render(_renderer, mouseX - tamBan.getW() / 2, mouseY - tamBan.getH() / 2, NULL, 0);
}

void TankMain::createBullet(SDL_Renderer* _renderer) {
	if (isMouseDown && !isMouseUp) {
		Bullet* bullet = new Bullet();
		bullet->loadImg("./image/danlua4.png", "./image/effect_shoot.png", "./image/collision3.png", _renderer);
		int x, y;
		if ((rotation >= 0 && rotation <= 90)) {
			bullet->setDir(Bullet::TOP_RIGHT);
			bullet->setSpX(sin((rotation * 3.14) / 180) * Bullet::speed);
			bullet->setSpY(cos((rotation * 3.14) / 180) * Bullet::speed);
			x = tankCircle.x + (sin((rotation * 3.14) / 180) * (box.h / 2)) - bullet->getW() / 2;
			y = tankCircle.y - (cos((rotation * 3.14) / 180) * (box.h / 2)) - bullet->getH() / 2;
		}
		else if (rotation > 90 && rotation <= 180) {
			bullet->setDir(Bullet::BOTTOM_RIGHT);
			bullet->setSpX(sin(3.14 - (rotation * 3.14) / 180) * Bullet::speed);
			bullet->setSpY(cos(3.14 - (rotation * 3.14) / 180) * Bullet::speed);
			x = tankCircle.x + (sin(3.14 - (rotation * 3.14) / 180) * (box.h / 2)) - bullet->getW() / 2;
			y = tankCircle.y + (cos(3.14 - (rotation * 3.14) / 180) * (box.h / 2)) - bullet->getH() / 2;
		}
		else if (rotation > 180 && rotation <= 270) {
			bullet->setDir(Bullet::BOTTOM_LEFT);
			bullet->setSpX(sin((rotation * 3.14) / 180 - 3.14) * Bullet::speed);
			bullet->setSpY(cos((rotation * 3.14) / 180 - 3.14) * Bullet::speed);
			x = tankCircle.x - (sin((rotation * 3.14) / 180 - 3.14) * (box.h / 2)) - bullet->getW() / 2;
			y = tankCircle.y + (cos((rotation * 3.14) / 180 - 3.14) * (box.h / 2)) - bullet->getH() / 2;
		}
		else if (rotation > 270 && rotation < 360) {
			bullet->setDir(Bullet::TOP_LEFT);
			bullet->setSpX(sin(6.28 - (rotation * 3.14) / 180) * Bullet::speed);
			bullet->setSpY(cos(6.28 - (rotation * 3.14) / 180) * Bullet::speed);
			x = tankCircle.x - (sin(6.28 - (rotation * 3.14) / 180) * (box.h / 2)) - bullet->getW() / 2;
			y = tankCircle.y - (cos(6.28 - (rotation * 3.14) / 180) * (box.h / 2)) - bullet->getH() / 2;
		}
		bullet->setXY(x, y);
		bullet->setRotation(rotation);
		bullet->setIsMove(true);

		bullets.push_back(bullet);
	}
}

void TankMain::handleBullet(MapGame map, SDL_Renderer* _renderer, SDL_Rect _camera) {
	for (int i = 0; i < bullets.size(); i++) {		
		bullets[i]->move();
		if (map.checkCollitionBullet(bullets[i]->getBox())) {
			bullets[i]->setIsMove(false);
			bullets[i]->renderCollisionEffect(_renderer, _camera);
			delete bullets[i];
			bullets.erase(bullets.begin() + i);
			i--;
		}
		/*if (bullets[i]->getIsMove() == false) {
			delete bullets[i];
			bullets.erase(bullets.begin() + i);
			i--;
		}*/
	}
}

void TankMain::renderBullet(SDL_Renderer* _renderer, SDL_Rect _camera) {
	for (int i = 0; i < bullets.size(); i++) {
		
		if (bullets[i]->getIsEffectShoot()) {
			bullets[i]->setIsEffectShoot(false);
			bullets[i]->renderShootEffect(_renderer, _camera, box);
		}
		bullets[i]->render(_renderer, _camera);
	}
}