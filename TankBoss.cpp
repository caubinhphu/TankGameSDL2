#include "TankBoss.h"


// -------------------------------- Tank boss ------------------------------------
TankBoss::TankBoss() {
	spX = spY = 0;
	rotation = 0;
	type = nomalTank;
	dir = TOP;
	bulletType = Bullet::nomalBullet;
	tankCircle = { 0, 0, 0 };
	isDestroy = false;
	speed = randomSpeed();
	bloodBar = { 0, 0, 60, 10, 40 };
	totalHealth = healthCurrent = 100;
	frameDestroy = 0;
	armor = 50;
	saveTimeShoot = 0;
	damgeReceive = 0;
	isDamgeReceived = 0;
}

TankBoss::~TankBoss() {
	;
}

void TankBoss::render(SDL_Renderer* _renderer, SDL_Rect _camera, TTF_Font* _font) {
	// render xe tank
	BasicObj::render(_renderer, box.x - _camera.x, box.y - _camera.y, NULL, rotation);

	// render thanh máu
	bloodBar.x = box.x;
	bloodBar.y = box.y - 20;
	SDL_SetRenderDrawColor(_renderer, 0, 0, 128, 0);
	SDL_Rect rimBar = { bloodBar.x - _camera.x, bloodBar.y - _camera.y, bloodBar.width, bloodBar.height };
	SDL_RenderDrawRect(_renderer, &rimBar);
	SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 0);
	bloodBar.percent = (healthCurrent * 100.0) / totalHealth; // tính phần trăm máu còn lại
	if (bloodBar.percent <= 0) bloodBar.percent = 0;
	SDL_Rect _bloodBar = { bloodBar.x - _camera.x + 1, bloodBar.y - _camera.y + 1, (bloodBar.width - 2) * (bloodBar.percent / 100.0), bloodBar.height - 2 };
	SDL_RenderFillRect(_renderer, &_bloodBar);

	// load + render minus health
	if (isDamgeReceived) {
		isDamgeReceived = false;
		SDL_Color _color = { 255, 0, 0 };
		std::stringstream _minus;
		_minus.str("");
		_minus << "- " << damgeReceive;
		textDamgeReceive.loadText(_font, _minus.str(), _color, _renderer);
		textDamgeReceive.render(_renderer, box.x + box.w - _camera.x, box.y - 20 - _camera.y, NULL, 0);
	}
}

int TankBoss::randomSpeed() {
	int _speed = 0;
	do {
		_speed = 1 + rand() % 5;
	} while (_speed == 3);
	return _speed;
}

void TankBoss::handleDirection()
{
	if (dir == LEFT) {
		spX = -speed;
		spY = 0;
	}
	else if (dir == RIGHT) {
		spX = speed;
		spY = 0;
	}
	else if (dir == TOP) {
		spY = -speed;
		spX = 0;
	}
	else if (dir == BOTTOM) {
		spY = speed;
		spX = 0;
	}
	else if (dir == STAND_STILL) {
		spX = spY = 0;
	}
}

void TankBoss::randomDirection(int k)
{
	int _direction;
	do
	{
		_direction = 1 + rand() % 5;
	} while (_direction == k);
	if (_direction == 1) dir = RIGHT;
	else if (_direction == 2) dir = LEFT;
	else if (_direction == 3) dir = TOP;
	else if (_direction == 4) dir = BOTTOM;
	else if (_direction == 5) dir = STAND_STILL;
}

void TankBoss::handleMove(MapGame _map, Circle _tankMain, bool isCollisionTeams, Circle _fireBall)
{
	rotation = check::rotationA_B(_tankMain.x, _tankMain.y, box.x + box.w / 2, box.y + box.h / 2);
	if (isCollisionTeams == true)
	{
		if (dir == LEFT)
		{
			spX = 5;
			spY = 0;
			dir = RIGHT;
		}
		else if (dir == RIGHT)
		{
			spX = -5;
			spY = 0;
			dir = LEFT;
		}
		else if (dir == TOP)
		{
			spY = 5;
			spX = 0;
			dir = BOTTOM;
		}
		else if (dir == BOTTOM)
		{
			spY = -5;
			spX = 0;
			dir = TOP;
		}
	}

	box.x += spX;
	setTankCircle(box.x, box.y);
	if (_map.checkCollision(tankCircle) ||
		check::checkCircle_Circle(tankCircle, _tankMain) ||
		check::checkCircle_Circle(tankCircle, _fireBall)) {

		box.x -= spX;
		setTankCircle(box.x, box.y);
		if (isCollisionTeams == false) randomDirection(0);
	}

	box.y += spY;
	setTankCircle(box.x, box.y);
	if (_map.checkCollision(tankCircle) ||
		check::checkCircle_Circle(tankCircle, _tankMain) ||
		check::checkCircle_Circle(tankCircle, _fireBall)) {

		box.y -= spY;
		setTankCircle(box.x, box.y);
		if (isCollisionTeams == false) randomDirection(0);
	}

	if (isCollisionTeams == false) // đổi hướng ngẫu nhiên
	{
		int _random = 1 + rand() % 50;
		if (_random == 1) randomDirection(0);
	}
}

void TankBoss::loadDestroyImg(SDL_Renderer* _renderer) {
	destroyImg[0].loadImg("./image/explosion_1.png", _renderer);
	destroyImg[1].loadImg("./image/explosion_2.png", _renderer);
	destroyImg[2].loadImg("./image/explosion_3.png", _renderer);
	destroyImg[3].loadImg("./image/explosion_4.png", _renderer);
}

bool TankBoss::renderDestroy(SDL_Renderer* _renderer, SDL_Rect _camera)
{
	// img_effect_money.render(_renderer, width_camera - img_effect_money.get_width() - 160, 10, NULL, 0);

	destroyImg[frameDestroy / 8].render(_renderer, box.x - _camera.x, box.y - _camera.y, NULL, 0);
	frameDestroy++;
	if (frameDestroy / 8 == 4)
		return true;
	return false;
}

void TankBoss::createBullet(SDL_Renderer* _renderer, Circle _tankMain)
{
	if (!isDestroy && check::binhPhuongKhoangCach(box.x, box.y, _tankMain.x, _tankMain.y) <= RANGE_SQUARE) {
		if (SDL_GetTicks() - saveTimeShoot > bulletRate) {
			Bullet* bullet = new Bullet(); // khai báo viên đạn mới
			bullet->setType(bulletType);
			bullet->loadImg(pathBulletImg, "./image/effect_shoot.png", "./image/collision3.png", _renderer);
			int x, y; // vị trí ban đầu
			if (rotation >= 0 && rotation <= PI / 2) {
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
			bullet->setDamge(bulletDamge);
			bullet->setIsMove(true);

			bullets.push_back(bullet);

			if (type == triangleTank) {
				for (int i = 0; i < 2; i++)
				{
					Bullet* _bullet = new Bullet();
					double rotationDeviation = 0;
					if (i == 0) {
						rotationDeviation = 0.35;
					}
					else if (i == 1) {
						rotationDeviation = -0.35;
					}
					double _rotation = rotation + rotationDeviation;
					if (_rotation < 0.0) _rotation = 2 * PI + _rotation;
					else if (_rotation > 2 * PI) _rotation -= 2 * PI;
					_bullet->setType(bulletType);
					_bullet->loadImg(pathBulletImg, "./image/effect_shoot.png", "./image/collision3.png", _renderer);
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
					_bullet->setDamge(bulletDamge);
					_bullet->setIsMove(true);

					bullets.push_back(_bullet);
				}
			 }
			saveTimeShoot = SDL_GetTicks();
		}
	}
}


int TankBoss::handleBullet(MapGame _map, SDL_Renderer* _renderer, SDL_Rect _camera, Circle _tankMain, bool& _isSlowedTankMain, Circle _fireBall) {
	int _damge = 0;
	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->move();

		if (_map.checkCollisionRect(bullets[i]->getBox())) {
			bullets[i]->setIsMove(false);
		}
		else if (check::checkRect_Circle(bullets[i]->getBox(), _tankMain)) {
			bullets[i]->setIsMove(false);
			_damge += bullets[i]->getDamge();
			if (type == iceTank) {
				_isSlowedTankMain = true;
			}
		}
		else if (check::checkRect_Circle(bullets[i]->getBox(), _fireBall)) {
			bullets[i]->setIsMove(false);
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

void TankBoss::renderBullet(SDL_Renderer* _renderer, SDL_Rect _camera) {
	for (int i = 0; i < bullets.size(); i++) {

		if (bullets[i]->getIsEffectShoot()) {
			bullets[i]->setIsEffectShoot(false);
			bullets[i]->renderShootEffect(_renderer, _camera, box);
		}
		bullets[i]->render(_renderer, _camera);
	}
}
















// -------------------------------- Tank boss list ---------------------------------
TankBossList::TankBossList() {
	;
}

TankBossList::~TankBossList() {
	;
}

void TankBossList::createListBoss(MapGame _map, Circle _tankMain, int _quality, int _typeNum, SDL_Renderer* _renderer, int _health, int _armor) {
	for (int i = 0; i < _quality; i++) {
		int type = 1 + rand() % _typeNum;
		TankBoss* boss = new TankBoss;
		if (type == 1) {
			boss->setType(TankBoss::nomalTank);
			boss->setBulletType(Bullet::nomalBullet);
			boss->loadImg("./image/tank_boss.png", _renderer);
			boss->setBulletDamge(Bullet::nomalDamge);
			boss->setBulletType(Bullet::nomalBullet);
			boss->setBulletRate(Bullet::nomalRate);
			boss->setPathBullet("./image/ammo.png");
			boss->setHealth(_health);
			boss->setArmor(_armor * 1.5);
		}
		else if (type == 2) {
			boss->setType(TankBoss::fireTank);
			boss->setBulletType(Bullet::fireBullet);
			boss->loadImg("./image/tank_boss_fire.png", _renderer);
			boss->setBulletDamge(Bullet::fireDamge);
			boss->setBulletType(Bullet::fireBullet);
			boss->setBulletRate(Bullet::fireRate);
			boss->setPathBullet("./image/danlua4.png");
			boss->setHealth(_health * 1.5);
			boss->setArmor(_armor);
		}
		else if (type == 3) {
			boss->setType(TankBoss::iceTank);
			boss->setBulletType(Bullet::iceBullet);
			boss->loadImg("./image/ice_tank.png", _renderer);
			boss->setBulletDamge(Bullet::iceDamge);
			boss->setBulletType(Bullet::iceBullet);
			boss->setBulletRate(Bullet::iceRate);
			boss->setPathBullet("./image/ice_bullet.png");
			boss->setHealth(_health);
			boss->setArmor(_armor);
		}
		else if (type == 4) {
			boss->setType(TankBoss::triangleTank);
			boss->setBulletType(Bullet::triangleBullet);
			boss->loadImg("./image/three_head_tank.png", _renderer);
			boss->setBulletDamge(Bullet::triangleDamge);
			boss->setBulletType(Bullet::triangleBullet);
			boss->setBulletRate(Bullet::triangleRate);
			boss->setPathBullet("./image/triangle_bullet.png");
			boss->setHealth(_health * 2);
			boss->setArmor(_armor * 2);
		}
		SDL_Rect _box = { 0, 0, boss->getW(), boss->getH() };
		Circle _bossCircle = { 0, 0, 0 };
		do {
			_box.x = TILE_WIDTH + rand() % (backgroundWidth - TILE_WIDTH - _box.w);
			_box.y = TILE_HEIGHT + rand() % (backgroundHeight - TILE_HEIGHT - _box.h);
			boss->setTankCircle(_box.x, _box.y);
			_bossCircle = boss->getTankCircle();
		} while (_map.checkCollision(_bossCircle) || checkCollisionTankBossList(_bossCircle, -1)
			|| check::checkCircle_Circle(_bossCircle, _tankMain)); // || _box.x % boss->getSpeed() != 0 || _box.y % boss->getSpeed() != 0
		boss->setXY(_box.x, _box.y);
		boss->loadDestroyImg(_renderer);
		bossList.push_back(boss);
	}
}

bool TankBossList::checkCollisionTankBossList(Circle _boss, int k)
{
	if (k != -1) {
		_boss.r += 10;
	}
	for (int i = 0; i < bossList.size(); i++)
	{
		Circle _boss2 = bossList[i]->getTankCircle();
		if (k != -1) {
			_boss2.r += 10;
		}
		if (i != k && check::checkCircle_Circle(_boss, _boss2)) {
			return true;
		}
	}
	return false;
}

void TankBossList::renderList(SDL_Renderer* _renderer, SDL_Rect _camera, TTF_Font* _font) {
	for (int i = 0; i < bossList.size(); i++) {
		if (check::checkRect_Rect(bossList[i]->getBox(), _camera) && !bossList[i]->getIsDestroy()) {
			bossList[i]->render(_renderer, _camera, _font);
		}
	}
}

void TankBossList::handleList(MapGame _map, Circle _tankMain, SDL_Renderer* _renderer, SDL_Rect _camera, ItemList _itemList, Circle _fireBall) {
	for (int i = 0; i < bossList.size(); i++) {
		if (!bossList[i]->getIsDestroy()) {
			bossList[i]->handleDirection();
			bossList[i]->createBullet(_renderer, _tankMain);
			bool isCollisionTeam = checkCollisionTankBossList(bossList[i]->getTankCircle(), i);
			bossList[i]->handleMove(_map, _tankMain, isCollisionTeam, _fireBall);
		}
		else {
			if (bossList[i]->renderDestroy(_renderer, _camera)) {
				/*Item* _item = new Item();
				int _type = 1 + rand() % TOTAL_ITEM_TYPE;
				if (_type == 1) {
					_item->loadImg("./image/bullet_item.png", _renderer);
					_item->setType(Item::fireBulletItem);
					_item->setTimeExists(TIME_EXISTS_FIREBULLET_ITEM);
				}
				else if (_type == 2) {
					_item->loadImg("./image/health_item.png", _renderer);
					_item->setType(Item::healthItem);
					_item->setTimeExists(TIME_EXISTS_HEALTH_ITEM);
				}
				else if (_type == 3) {
					_item->loadImg("./image/money_item.png", _renderer);
					_item->setType(Item::moneyItem);
					_item->setTimeExists(TIME_EXISTS_MONEY_ITEM);
				}
				_item->setXY(bossList[i]->getTankCircle().x - _item->getW() / 2, bossList[i]->getTankCircle().y - _item->getH() / 2);
				_item->setTimeCreate(SDL_GetTicks());*/

				// _itemList.setList(_item);
				//_itemList.createItemListFromBossDestroy(_renderer, bossList[i]->getTankCircle());


				//std::cout <<_itemList.getSize() << std::endl;
				delete bossList[i];
			//	std::cout << _itemList.getSize() << std::endl;
				bossList.erase(bossList.begin() + i);
		//		std::cout << _itemList.getSize() << std::endl;
				i--;
			}
		}
	}
	// if (_itemList.getSize() != 0) std::cout << _itemList.getSize() << std::endl;
}

bool TankBossList::checkCollisionBullet(SDL_Rect _bullet, bool _iSenemies, int _damgeBullet)
{
	if (_iSenemies)
	{
		for (int i = 0; i < bossList.size(); i++)
		{

			if (check::checkRect_Circle(_bullet, bossList[i]->getTankCircle()))
			{
				int _damge = _damgeBullet * (1 - (bossList[i]->getArmor() / 100.0));

				bossList[i]->setIsDamgeReceived(true);
				bossList[i]->setDamgeReceive(_damge);

				bossList[i]->setMinusHealthCurrent(_damge);
				if (bossList[i]->getHealteCurrent() <= 0) // kiểm tra xe boss còn sống?
				{
					//ds_boss[i]->set_sound_destroy(true);
					bossList[i]->setIsDestroy(true);
				}
				return true;
			}
		}
	}
	return false;
}

int TankBossList::handleBulletOfTankList(MapGame _map, SDL_Renderer* _renderer, SDL_Rect _camera, Circle _tankMain, bool& _isSlowedTankMain, Circle _fireBall) {
	int _damge = 0;
	for (int i = 0; i < bossList.size(); i++) {
		_damge += bossList[i]->handleBullet(_map, _renderer, _camera, _tankMain, _isSlowedTankMain, _fireBall);
	}
	return _damge;
}

void TankBossList::renderBulletOfTankList(SDL_Renderer* _renderer, SDL_Rect _camera) {
	for (int i = 0; i < bossList.size(); i++) {
		bossList[i]->renderBullet(_renderer, _camera);
	}
}