#include "TankMain.h"

TankMain::TankMain(int _x, int _y) {
	box = { _x, _y, TANK_WIDTH, TANK_HEIGHT };
	speed = 5;
	spX = spY = 0;
	rotation = 0;
	setTankCircle();
	isMouseDown = isMouseUp = false;
	saveTimeShoot = 0;
	bulletType = Bullet::BulletType::nomalBullet;
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
	// std::cout << rotation << std::endl;

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
		case SDLK_1:
			bulletType = Bullet::nomalBullet; break;
		case SDLK_2:
			bulletType = Bullet::fireBullet; break;
		case SDLK_3:
			bulletType = Bullet::rocketBullet; break;
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

void TankMain::move(MapGame map, TankBossList _bossList) {
	box.x += spX;
	box.y += spY;
	setTankCircle();
	if (map.checkCollision(tankCircle) || _bossList.checkCollisionTankBossList(tankCircle, -1)) {
		box.x -= spX;
		setTankCircle();
	}
	if (map.checkCollision(tankCircle) || _bossList.checkCollisionTankBossList(tankCircle, -1)) {
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
	if (isMouseDown && !isMouseUp) { // kiểm tra chuột trái có đang được nhấn
		Bullet::BulletFirtingRate firtingRate;

		// Kiểm tra tốc độ bắn của loại đạn đan sử dung
		if (bulletType == Bullet::nomalBullet) {
			firtingRate = Bullet::nomalRate;
		}
		else if (bulletType == Bullet::fireBullet) {
			firtingRate = Bullet::fireRate;
		}
		else if (bulletType == Bullet::rocketBullet) {
			firtingRate = Bullet::rocketRate;
		}

		// kiểm tra thời điểm bắn đạn trước so với thời điểm bắn đạn sau có lớn hơn tốc độ bắn hay không
		if (SDL_GetTicks() - saveTimeShoot > firtingRate) {
			std::string _imgBullet; // hình viên đạn
			if (bulletType == Bullet::nomalBullet) {
				_imgBullet = "./image/ammo.png";
				
			}
			else if (bulletType == Bullet::fireBullet) {
				_imgBullet = "./image/danlua4.png";
			}
			else if (bulletType == Bullet::rocketBullet) {
				_imgBullet = "./image/bullet_rocket.png";
			}

			Bullet* bullet = new Bullet(); // khai báo viên đạn mới
			bullet->setType(bulletType);
			bullet->loadImg(_imgBullet, "./image/effect_shoot.png", "./image/collision3.png", _renderer);
			int x, y; // vị trí ban đầu
			if ((rotation >= 0 && rotation <= PI / 2)) {
				bullet->setDir(Bullet::TOP_RIGHT);
				bullet->setSpX(round(sin(rotation) * Bullet::speed)); // độ lệch x
				bullet->setSpY(round(cos(rotation) * Bullet::speed)); // độ lệch y
				x = tankCircle.x + (sin(rotation) * (box.h / 2)) - bullet->getW() / 2;
				y = tankCircle.y - (cos(rotation) * (box.h / 2)) - bullet->getH() / 2;
				// std::cout << (sin(rotation) * Bullet::speed) << (cos(rotation) * Bullet::speed);
			}
			else if (rotation > PI / 2 && rotation <= PI) {
				bullet->setDir(Bullet::BOTTOM_RIGHT);
				bullet->setSpX(round(sin(PI - rotation) * Bullet::speed));
				bullet->setSpY(round(cos(PI - rotation) * Bullet::speed));
				x = tankCircle.x + (sin(PI - rotation) * (box.h / 2)) - bullet->getW() / 2;
				y = tankCircle.y + (cos(PI - rotation) * (box.h / 2)) - bullet->getH() / 2;
			}
			else if (rotation > PI && rotation <= 1.5 * PI) {
				bullet->setDir(Bullet::BOTTOM_LEFT);
				bullet->setSpX(round(sin(rotation - PI) * Bullet::speed));
				bullet->setSpY(round(cos(rotation - PI) * Bullet::speed));
				x = tankCircle.x - (sin(rotation - PI) * (box.h / 2)) - bullet->getW() / 2;
				y = tankCircle.y + (cos(rotation - PI) * (box.h / 2)) - bullet->getH() / 2;
			}
			else if (rotation > 1.5 * PI && rotation < 2 * PI) {
				bullet->setDir(Bullet::TOP_LEFT);
				bullet->setSpX(round(sin(2 * PI - rotation) * Bullet::speed));
				bullet->setSpY(round(cos(2 * PI - rotation) * Bullet::speed));
				x = tankCircle.x - (sin(2 * PI - rotation) * (box.h / 2)) - bullet->getW() / 2;
				y = tankCircle.y - (cos(2 * PI - rotation) * (box.h / 2)) - bullet->getH() / 2;
			}
			bullet->setXY(x, y);
			bullet->setRotation(rotation);
			bullet->setIsMove(true);

			bullets.push_back(bullet);
			saveTimeShoot = SDL_GetTicks();
		}
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