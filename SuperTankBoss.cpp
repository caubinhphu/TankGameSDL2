#include "SuperTankBoss.h"

SuperTankBoss::SuperTankBoss() {
	spX = spY = 0;
	rotation = 0;
	speed = 1;
	totalHealth = healthCurrent = 1000;
	armor = 0;
	bloodBar = { 0, 0, 205, 15, 100 };
	damgeReceived = 0;
	isMinusHealth = false;
	saveTimeShoot = timeBulletLv1 = timeBulletLv2 = 0;
	isAllowRenderShield = false;
	isSwitchLevel = false;
	switchLevel = -1;
	frameDestroy = 0;
}

SuperTankBoss::~SuperTankBoss() {
	;
}

void SuperTankBoss::setTankCircle(int _x, int _y) {
	tankCircle.x = _x + box.w / 2;
	tankCircle.y = _y + box.h / 2;
	tankCircle.r = box.w / 2;
}

void SuperTankBoss::loadEffectAppear(SDL_Renderer* _renderer) {
	effectAppear[0].loadImg("./image/effect_appear_1.png", _renderer);
	effectAppear[1].loadImg("./image/effect_appear_2.png", _renderer);
	effectAppear[2].loadImg("./image/effect_appear_3.png", _renderer);
	effectAppear[3].loadImg("./image/effect_appear_4.png", _renderer);
	effectAppear[4].loadImg("./image/effect_appear_5.png", _renderer);
}

void SuperTankBoss::handleEffectAppear(std::vector<int> _mapWall) {
	for (int i = 0; i < TOTAL_TILE; i++) {
		int hang = 0, cot = 0;
		if (_mapWall[i] == 1) {
			hang = i / 20;
			cot = i % 20;
			if (hang != 0 && hang != 19 && cot != 0 && cot != 19) {
				std::vector<int> tam;
				tam.push_back(hang);
				tam.push_back(cot);
				positionWallTileList.push_back(tam);
			}
		}
	}
}

bool SuperTankBoss::renderEffectAppear(SDL_Renderer* _renderer, SDL_Rect _camera) {
	int x = 0, y = 0;
	for (int i = 0; i < positionWallTileList.size(); i++)
	{
		y = positionWallTileList[i][0] * 80;
		x = positionWallTileList[i][1] * 80;
		effectAppear[frame / 20].render(_renderer, x + 40 - effectAppear[frame / 20].getW() / 2 - _camera.x, y - _camera.y, NULL, 0);
	}
	frame++;
	if (frame / 20 != 5) return false;
	return true;
}

void SuperTankBoss::handleMoveAppear() {
	spX = 0;
	spY = 2;
}

bool SuperTankBoss::moveAuto(int _y) {
	box.x += spX;
	box.y += spY;
	if (box.y >= _y)
		return true;
	return false;
}

void SuperTankBoss::handleMove(Circle _tankMain, int _y) {
	rotation = check::rotationA_B(_tankMain.x, _tankMain.y, box.x + box.w / 2, box.y + box.h / 2);
	//handle
	if (box.y != _y) {
		spX = 0;
		if (box.y > _y) spY = -speed;
		else if (box.y < _y) spY = speed;
	}
	else {
		spX = spY = 0;
	}
}

void SuperTankBoss::handleSwitchLevel(Circle _tankMain) {
	if (bloodBar.percent > 95) {
		// bắn đạn lửa
		handleMove(_tankMain, backgroundHeight / 2 - box.h / 2);
		bulletType = Bullet::fireBossBullet;
	}
	else if (bloodBar.percent > 85) {
		// bắn đạn lửa
		// sinh boss
		handleMove(_tankMain, 85);
		bulletType = Bullet::fireBossBullet;
		if (switchLevel < 0)
		{
			switchLevel++;
			isSwitchLevel = true;
		}
	}
	else if (bloodBar.percent > 75) {
		// bắn đạn cầu 1
		// sinh boss
		handleMove(_tankMain, backgroundHeight / 2 - box.h / 2);
		bulletType = Bullet::ballNomalBullet;
		if (switchLevel < 1)
		{
			switchLevel++;
			isSwitchLevel = true;
		}
	}
	else if (bloodBar.percent > 55) {
		// bắn đạn lửa
		// sinh boss
		handleMove(_tankMain, backgroundHeight - 85 - box.h);
		bulletType = Bullet::fireBossBullet;
		if (switchLevel < 2)
		{
			switchLevel++;
			isSwitchLevel = true;
		}
	}
	else if (bloodBar.percent > 45) {
		// bắn đạn cầu 2
		handleMove(_tankMain, backgroundHeight / 2 - box.h / 2);
		bulletType = Bullet::ballBullet;
	}
	else if (bloodBar.percent > 10) {
		// tăng giáp
		// bắn đạn bi
		// sinh boss
		handleMove(_tankMain, backgroundHeight / 2 - 2 * box.h);
		bulletType = Bullet::marblesLv1Bullet;
		armor = 50;
		isAllowRenderShield = true;

		if (switchLevel < 3)
		{
			switchLevel++;
			isSwitchLevel = true;
		}
	}
	else if (bloodBar.percent > 0) {
		// tăng giáp
		// bắn đạn bi
		// sinh boss
		handleMove(_tankMain, backgroundHeight / 2 - box.h / 2);
		bulletType = Bullet::ballBullet;
		armor = 80;
		if (switchLevel < 4)
		{
			switchLevel++;
			isSwitchLevel = true;
		}
	}
	else if (bloodBar.percent <= 0)
		isDestroy = true;
}

void SuperTankBoss::move() {
	box.x += spX;
	box.y += spY;
	setTankCircle(box.x, box.y);
	setCircleBallFire(box.x - 45, box.y - 45);
}

bool SuperTankBoss::loadImg(SDL_Renderer* _renderer) {
	if (!BasicObj::loadImg("./image/super_tank_boss_2.png", _renderer))
		return false;
	if (!ballFire.loadImg("./image/ball_fire_3.png", _renderer))
		return false;
	if (!shield.loadImg("./image/shield_2.png", _renderer))
		return false;
	return true;
}

bool SuperTankBoss::render(SDL_Renderer* _renderer, SDL_Rect _camera) {
	if (!isDestroy) {
		// render tank
		BasicObj::render(_renderer, box.x - _camera.x, box.y - _camera.y, NULL, rotation);

		// render vòng lửa
		ballFire.render(_renderer, box.x - 45 - _camera.x, box.y - 45 - _camera.y, NULL, 0);

		// render shield nếu có
		if (isAllowRenderShield)
			shield.render(_renderer, box.x - _camera.x, box.y - _camera.y, NULL, rotation);

		// render thanh máu
		bloodBar.x = box.x;
		bloodBar.y = box.y - 65;
		SDL_SetRenderDrawColor(_renderer, 0, 0, 128, 0);
		SDL_Rect rimBar = { bloodBar.x - _camera.x, bloodBar.y - _camera.y, bloodBar.width, bloodBar.height };
		SDL_RenderDrawRect(_renderer, &rimBar);
		SDL_SetRenderDrawColor(_renderer, 77, 1, 201, 0);
		bloodBar.percent = healthCurrent * 100 / totalHealth; // tính phần trăm máu còn lại
		if (bloodBar.percent <= 0) bloodBar.percent = 0;
		SDL_Rect _bloodBar = { bloodBar.x - _camera.x + 1, bloodBar.y - _camera.y + 1, (float)(bloodBar.width - 2) * ((float)bloodBar.percent / 100), bloodBar.height - 2 };
		SDL_RenderFillRect(_renderer, &_bloodBar);

		// render text máu mất
		if (isMinusHealth) {
			isMinusHealth = false;
			textMinusHealth.render(_renderer, box.x + box.w - _camera.x + 5, box.y - 65 - _camera.y, NULL, 0);
		}
	}
	else {
		spX = spY = 0;
		if (renderDestroy(_renderer, _camera)) {
			return false; // you win
		}
	}
	return true;
}

void SuperTankBoss::handleDamgeReceived(SDL_Renderer* _renderer, TTF_Font* _font) {
	if (damgeReceived != 0) {
		int _damge = damgeReceived * (1 - (armor / 100.0));
		// std::cout << _damgeReceived << ", " << _damge << std::endl;
		healthCurrent -= _damge;
		isMinusHealth = true;
		// load text minus health
		SDL_Color _color = { 255, 0, 0 };
		std::stringstream _minus;
		_minus.str("");
		_minus << "- " << _damge;
		textMinusHealth.loadText(_font, _minus.str(), _color, _renderer);
		damgeReceived = 0;
	}
}

void SuperTankBoss::createBullet(SDL_Renderer* _renderer) {
	if (!isDestroy) {
		Bullet::BulletFirtingRate firtingRate{};
		if (bulletType == Bullet::fireBossBullet) {
			firtingRate = Bullet::fireBossRate;
		}
		else if (bulletType == Bullet::ballBullet) {
			firtingRate = Bullet::ballBossRate;
		}
		else if (bulletType == Bullet::ballNomalBullet) {
			firtingRate = Bullet::ballNormalBossRate;
		}
		else if (bulletType == Bullet::marblesLv1Bullet) {
			firtingRate = Bullet::marblesRate;
		}


		if (SDL_GetTicks() - saveTimeShoot > firtingRate) {
			std::string _imgBullet; // hình viên đạn
			Bullet::DameBullet _damge = Bullet::fireBossDamge; // damge viên đạn
			if (bulletType == Bullet::fireBossBullet) {
				_imgBullet = "./image/bullet_tank_boss.png";
				_damge = Bullet::fireBossDamge;

			}
			else if (bulletType == Bullet::ballNomalBullet) {
				_imgBullet = "./image/bullet_tank_boss_2.png";
				_damge = Bullet::ballNormalBossDamge;
			}
			else if (bulletType == Bullet::ballBullet) {
				_imgBullet = "./image/bullet_tank_boss_2.png";
				_damge = Bullet::ballBossDamge;
			}
			else if (bulletType == Bullet::marblesLv1Bullet) {
				_imgBullet = "./image/marbles_bullet_lv1.png";
				_damge = Bullet::marblesDamge;
			}

			Bullet* bullet = new Bullet(); // khai báo viên đạn mới
			bullet->setType(bulletType);
			bullet->loadImg(_imgBullet, "./image/effect_shoot_2.png", "./image/collision3.png", _renderer);
			int x, y; // vị trí ban đầu
			int spXBullet, spYBullet;
			double rotationBullet;
			Bullet::ShootDir bulletDir;
			if (bulletType == Bullet::ballNomalBullet || bulletType == Bullet::ballBullet) {
				x = box.x + box.w / 2 - bullet->getW() / 2;
				y = box.y - bullet->getH() / 2;
				rotationBullet = 0;
				spXBullet = 0;
				spYBullet = Bullet::speed;
				bulletDir = Bullet::TOP_RIGHT;
			}
			else {
				if (rotation >= 0 && rotation <= PI / 2) {
					bulletDir = Bullet::TOP_RIGHT;
					spXBullet = round(sin(rotation) * Bullet::speed); // độ lệch x
					spYBullet = round(cos(rotation) * Bullet::speed); // độ lệch y
					x = tankCircle.x + (sin(rotation) * (box.h / 2)) - bullet->getW() / 2;
					y = tankCircle.y - (cos(rotation) * (box.h / 2)) - bullet->getH() / 2;
					// std::cout << (sin(rotation) * Bullet::speed) << (cos(rotation) * Bullet::speed);
				}
				else if (rotation > PI / 2 && rotation <= PI) {
					bulletDir = Bullet::BOTTOM_RIGHT;
					spXBullet = round(sin(PI - rotation) * Bullet::speed);
					spYBullet = round(cos(PI - rotation) * Bullet::speed);
					x = tankCircle.x + (sin(PI - rotation) * (box.h / 2)) - bullet->getW() / 2;
					y = tankCircle.y + (cos(PI - rotation) * (box.h / 2)) - bullet->getH() / 2;
				}
				else if (rotation > PI&& rotation <= 1.5 * PI) {
					bulletDir = Bullet::BOTTOM_LEFT;
					spXBullet = round(sin(rotation - PI) * Bullet::speed);
					spYBullet = round(cos(rotation - PI) * Bullet::speed);
					x = tankCircle.x - (sin(rotation - PI) * (box.h / 2)) - bullet->getW() / 2;
					y = tankCircle.y + (cos(rotation - PI) * (box.h / 2)) - bullet->getH() / 2;
				}
				else if (rotation > 1.5 * PI && rotation < 2 * PI) {
					bulletDir = Bullet::TOP_LEFT;
					spXBullet = round(sin(2 * PI - rotation) * Bullet::speed);
					spYBullet = round(cos(2 * PI - rotation) * Bullet::speed);
					x = tankCircle.x - (sin(2 * PI - rotation) * (box.h / 2)) - bullet->getW() / 2;
					y = tankCircle.y - (cos(2 * PI - rotation) * (box.h / 2)) - bullet->getH() / 2;
				}
				rotationBullet = rotation;
			}

			bullet->setDir(bulletDir);
			bullet->setXY(x, y);
			bullet->setSpX(spXBullet);
			bullet->setSpY(spYBullet);
			bullet->setRotation(rotationBullet);
			bullet->setDamge(_damge);
			bullet->setIsMove(true);
			if (bulletType == Bullet::marblesLv1Bullet)
				bullet->setTimeShoot(SDL_GetTicks());

			bullets.push_back(bullet);

			if (bulletType == Bullet::ballNomalBullet || bulletType == Bullet::ballBullet) {
				int _quantity = 0;
				float _angleDeviation = 0.0;
				float _rotation = 0.0;
				if (bulletType == Bullet::ballBullet) {
					_quantity = 35;
					_angleDeviation = PI / 18;
				}
				else {
					_quantity = 14;
					_angleDeviation = (2 * PI) / 15;
				}

				for (int i = 1; i <= _quantity; i++) {
					Bullet* _bullet = new Bullet();
					_rotation = i * _angleDeviation;
					if (_rotation < 0.0) _rotation = 2 * PI + _rotation;
					else if (_rotation > 2 * PI) _rotation -= 2 * PI;
					_bullet->setType(bulletType);
					_bullet->loadImg("./image/bullet_tank_boss_2.png", "./image/effect_shoot_2.png", "./image/collision3.png", _renderer);
					int _x{}, _y{}; // vị trí ban đầu
					if (_rotation >= 0 && _rotation <= PI / 2) {
						_bullet->setDir(Bullet::TOP_RIGHT);
						_bullet->setSpX(round(sin(_rotation) * Bullet::speed)); // độ lệch x
						_bullet->setSpY(round(cos(_rotation) * Bullet::speed)); // độ lệch y
						_x = tankCircle.x + (sin(_rotation) * (box.h / 2)) - _bullet->getW() / 2;
						_y = tankCircle.y - (cos(_rotation) * (box.h / 2)) - _bullet->getH() / 2;
					}
					else if (_rotation > PI / 2 && _rotation <= PI) {
						_bullet->setDir(Bullet::BOTTOM_RIGHT);
						_bullet->setSpX(round(sin(PI - _rotation) * Bullet::speed));
						_bullet->setSpY(round(cos(PI - _rotation) * Bullet::speed));
						_x = tankCircle.x + (sin(PI - _rotation) * (box.h / 2)) - _bullet->getW() / 2;
						_y = tankCircle.y + (cos(PI - _rotation) * (box.h / 2)) - _bullet->getH() / 2;
					}
					else if (_rotation > PI&& _rotation <= 1.5 * PI) {
						_bullet->setDir(Bullet::BOTTOM_LEFT);
						_bullet->setSpX(round(sin(_rotation - PI) * Bullet::speed));
						_bullet->setSpY(round(cos(_rotation - PI) * Bullet::speed));
						_x = tankCircle.x - (sin(_rotation - PI) * (box.h / 2)) - _bullet->getW() / 2;
						_y = tankCircle.y + (cos(_rotation - PI) * (box.h / 2)) - _bullet->getH() / 2;
					}
					else if (_rotation > 1.5 * PI && _rotation < 2 * PI) {
						_bullet->setDir(Bullet::TOP_LEFT);
						_bullet->setSpX(round(sin(2 * PI - _rotation) * Bullet::speed));
						_bullet->setSpY(round(cos(2 * PI - _rotation) * Bullet::speed));
						_x = tankCircle.x - (sin(2 * PI - _rotation) * (box.h / 2)) - _bullet->getW() / 2;
						_y = tankCircle.y - (cos(2 * PI - _rotation) * (box.h / 2)) - _bullet->getH() / 2;
					}
					_bullet->setXY(_x, _y);
					_bullet->setRotation(_rotation);
					if (bulletType == Bullet::ballNomalBullet) _bullet->setDamge(Bullet::ballNormalBossDamge);
					else if (bulletType == Bullet::ballBullet) _bullet->setDamge(Bullet::ballBossDamge);
					_bullet->setIsMove(true);

					bullets.push_back(_bullet);
				}
			}

			saveTimeShoot = SDL_GetTicks();
		}
	}
}

int SuperTankBoss::handleBullet(MapGame _map, SDL_Renderer* _renderer, SDL_Rect _camera, Circle _tankMain) {
	int _damge = 0;
	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->move();

		if (_map.checkCollisionRect(bullets[i]->getBox())) {
			bullets[i]->setIsMove(false);
		}
		else if (check::checkRect_Circle(bullets[i]->getBox(), _tankMain)) {
			bullets[i]->setIsMove(false);
			_damge += bullets[i]->getDamge();
		}

		if (bullets[i]->getType() == Bullet::marblesLv1Bullet || bullets[i]->getType() == Bullet::marblesLv2Bullet) {
			if (SDL_GetTicks() - bullets[i]->getTimeShoot() >= 200) {
				bullets[i]->setIsMove(false);
				if (bullets[i]->getType() == Bullet::marblesLv1Bullet)
					createMarblesBullet(2, _renderer, bullets[i]->getBox(), bullets[i]->getRotation());
				else if (bullets[i]->getType() == Bullet::marblesLv2Bullet)
					createMarblesBullet(3, _renderer, bullets[i]->getBox(), bullets[i]->getRotation());
			}
		}

		if (bullets[i]->getIsMove() == false) {
			bullets[i]->renderCollisionEffect(_renderer, _camera);
			delete bullets[i];
			bullets.erase(bullets.begin() + i);
			i--;
		}
	}
	return _damge;
}

void SuperTankBoss::renderBullet(SDL_Renderer* _renderer, SDL_Rect _camera) {
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i]->getIsEffectShoot()) {
			bullets[i]->setIsEffectShoot(false);
			bullets[i]->renderShootEffect(_renderer, _camera, box);
		}
		bullets[i]->render(_renderer, _camera);
	}
}

void SuperTankBoss::createMarblesBullet(int _marblesLv, SDL_Renderer* _renderer, SDL_Rect _parentBullet, double _rotationParentBullet) {
	int x = 0, y = 0;
	double _angleDeviation = 0.0;
	double _rotation = 0.0;
	for (int i = 0; i < 3; i++) {
		Bullet* bullet = new Bullet();
		
		if (i == 0) _angleDeviation = PI / 18;
		else if (i == 1) _angleDeviation = -(PI / 18);
		else if (i == 2) _angleDeviation = 0.0;

		_rotation = _rotationParentBullet + _angleDeviation;
		if (_rotation < 0.0) _rotation = 2 * PI + _rotation;
		else if (_rotation > 2 * PI) _rotation -= 2 * PI;

		if (_marblesLv == 2) {
			bullet->loadImg("./image/marbles_bullet_lv2.png", "./image/effect_shoot_2.png", "./image/collision3.png", _renderer);
			bullet->setType(Bullet::marblesLv2Bullet);
		}
		else if (_marblesLv == 3) {
			bullet->loadImg("./image/marbles_bullet_lv3.png", "./image/effect_shoot_2.png", "./image/collision3.png", _renderer);
			bullet->setType(Bullet::marblesLv3Bullet);
		}

		if (_rotation >= 0 && _rotation <= PI / 2) {
			bullet->setDir(Bullet::TOP_RIGHT);
			bullet->setSpX(round(sin(_rotation) * Bullet::speed)); // độ lệch x
			bullet->setSpY(round(cos(_rotation) * Bullet::speed)); // độ lệch y
			x = _parentBullet.x + _parentBullet.w / 2 + (sin(_rotation) * (_parentBullet.h / 2)) - bullet->getW() / 2;
			y = _parentBullet.y + _parentBullet.h / 2 - (cos(_rotation) * (_parentBullet.h / 2)) - bullet->getH() / 2;
		}
		else if (_rotation > PI / 2 && _rotation <= PI) {
			bullet->setDir(Bullet::BOTTOM_RIGHT);
			bullet->setSpX(round(sin(PI - _rotation) * Bullet::speed));
			bullet->setSpY(round(cos(PI - _rotation) * Bullet::speed));
			x = _parentBullet.x + _parentBullet.w / 2 + (sin(PI - _rotation) * (_parentBullet.h / 2)) - bullet->getW() / 2;
			y = _parentBullet.y + _parentBullet.h / 2 + (cos(PI - _rotation) * (_parentBullet.h / 2)) - bullet->getH() / 2;
		}
		else if (_rotation > PI&& _rotation <= 1.5 * PI) {
			bullet->setDir(Bullet::BOTTOM_LEFT);
			bullet->setSpX(round(sin(_rotation - PI) * Bullet::speed));
			bullet->setSpY(round(cos(_rotation - PI) * Bullet::speed));
			x = _parentBullet.x + _parentBullet.w / 2 - (sin(_rotation - PI) * (_parentBullet.h / 2)) - bullet->getW() / 2;
			y = _parentBullet.y + _parentBullet.h / 2 + (cos(_rotation - PI) * (_parentBullet.h / 2)) - bullet->getH() / 2;
		}
		else if (_rotation > 1.5 * PI && _rotation < 2 * PI) {
			bullet->setDir(Bullet::TOP_LEFT);
			bullet->setSpX(round(sin(2 * PI - _rotation) * Bullet::speed));
			bullet->setSpY(round(cos(2 * PI - _rotation) * Bullet::speed));
			x = _parentBullet.x + _parentBullet.w / 2 - (sin(2 * PI - _rotation) * (_parentBullet.h / 2)) - bullet->getW() / 2;
			y = _parentBullet.y + _parentBullet.h / 2 - (cos(2 * PI - _rotation) * (_parentBullet.h / 2)) - bullet->getH() / 2;
		}
		bullet->setXY(x, y);
		bullet->setRotation(_rotation);
		bullet->setIsMove(true);
		bullet->setTimeShoot(SDL_GetTicks());

		bullets.push_back(bullet);
	}
}

void SuperTankBoss::setCircleBallFire(int _x, int _y) {
	ballFireCircle.x = _x + ballFire.getW() / 2;
	ballFireCircle.y = _y + ballFire.getH() / 2;
	ballFireCircle.r = ballFire.getW() / 2;
}

void SuperTankBoss::loadDestroyImg(SDL_Renderer* _renderer) {
	destroyImg[0].loadImg("./image/explosionBoss_1.png", _renderer);
	destroyImg[1].loadImg("./image/explosionBoss_2.png", _renderer);
	destroyImg[2].loadImg("./image/explosionBoss_3.png", _renderer);
	destroyImg[3].loadImg("./image/explosionBoss_4.png", _renderer);
}

bool SuperTankBoss::renderDestroy(SDL_Renderer* _renderer, SDL_Rect _camera)
{
	destroyImg[frameDestroy / 8].render(_renderer, box.x - _camera.x, box.y - _camera.y, NULL, 0);
	frameDestroy++;
	if (frameDestroy / 8 == 4)
		return true;
	return false;
}