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
}

TankBoss::~TankBoss() {
	;
}

void TankBoss::render(SDL_Renderer* _renderer, SDL_Rect _camera) {
	// render xe tank
	BasicObj::render(_renderer, box.x - _camera.x, box.y - _camera.y, NULL, rotation);

	// render thanh máu
	bloodBar.x = box.x;
	bloodBar.y = box.y - 20;
	SDL_SetRenderDrawColor(_renderer, 0, 0, 128, 0);
	SDL_Rect rimBar = { bloodBar.x - _camera.x, bloodBar.y - _camera.y, bloodBar.width, bloodBar.height };
	SDL_RenderDrawRect(_renderer, &rimBar);
	SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 0);
	bloodBar.percent = healthCurrent * 100 / totalHealth; // tính phần trăm máu còn lại
	if (bloodBar.percent <= 0) bloodBar.percent = 0;
	SDL_Rect _bloodBar = { bloodBar.x - _camera.x + 1, bloodBar.y - _camera.y + 1, (float)(bloodBar.width - 2) * ((float)bloodBar.percent / 100), bloodBar.height - 2 };
	SDL_RenderFillRect(_renderer, &_bloodBar);
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
	if (dir == LEFT)
	{
		spX = -speed;
		spY = 0;
	}
	else if (dir == RIGHT)
	{
		spX = speed;
		spY = 0;
	}
	else if (dir == TOP)
	{
		spY = -speed;
		spX = 0;
	}
	else if (dir == BOTTOM)
	{
		spY = speed;
		spX = 0;
	}
}

void TankBoss::randomDirection(int k)
{
	int _direction;
	do
	{
		_direction = 1 + rand() % 4;
	} while (_direction == k);
	if (_direction == 1) dir = RIGHT;
	else if (_direction == 2) dir = LEFT;
	else if (_direction == 3) dir = TOP;
	else if (_direction == 4) dir = BOTTOM;
}

void TankBoss::handleMove(MapGame _map, Circle _tankMain, bool isCollisionTeams)
{
	rotation = check::rotationA_B(_tankMain.x, _tankMain.y, box.x + box.w / 2, box.y + box.h / 2);

	if (isCollisionTeams == true)
	{
		if (dir == LEFT)
		{
			spX = 6;
			spY = 0;
			dir = RIGHT;
		}
		else if (dir == RIGHT)
		{
			spX = -6;
			spY = 0;
			dir = LEFT;
		}
		else if (dir == TOP)
		{
			spY = 6;
			spX = 0;
			dir = BOTTOM;
		}
		else if (dir == BOTTOM)
		{
			spY = -6;
			spX = 0;
			dir = TOP;
		}
	}

	box.x += spX;
	setTankCircle(box.x, box.y);
	if (_map.checkCollision(tankCircle) || check::checkCircle_Circle(tankCircle, _tankMain))
	{
		box.x -= spX;
		setTankCircle(box.x, box.y);
		if (isCollisionTeams == false) randomDirection(0);
	}

	box.y += spY;
	setTankCircle(box.x, box.y);
	if (_map.checkCollision(tankCircle) || check::checkCircle_Circle(tankCircle, _tankMain))
	{
		box.y -= spY;
		setTankCircle(box.x, box.y);
		if (isCollisionTeams == false) randomDirection(0);
	}

	if (isCollisionTeams == false)
	{
		int _random = 1 + rand() % 50;
		if (_random == 1) randomDirection(0);
	}
}






// -------------------------------- Tank boss list ---------------------------------
TankBossList::TankBossList() {
	;
}

TankBossList::~TankBossList() {
	;
}

void TankBossList::createListBoss(MapGame _map, Circle _tankMain, int _quality, int _typeNum, SDL_Renderer* _renderer) {
	for (int i = 0; i < _quality; i++) {
		int type = 1 + rand() % _typeNum;
		TankBoss* boss = new TankBoss;
		if (type == 1) {
			boss->setType(TankBoss::nomalTank);
			boss->setBulletType(Bullet::nomalBullet);
			boss->loadImg("./image/tank_boss.png", _renderer);
		}
		else if (type == 2) {
			boss->setType(TankBoss::fireTank);
			boss->setBulletType(Bullet::fireBullet);
			boss->loadImg("./image/tank_boss_fire.png", _renderer);
		}
		else if (type == 3) {
			boss->setType(TankBoss::iceTank);
			boss->setBulletType(Bullet::iceBullet);
			boss->loadImg("./image/ice_tank.png", _renderer);
		}
		else if (type == 4) {
			boss->setType(TankBoss::triangleTank);
			boss->setBulletType(Bullet::triangleBullet);
			boss->loadImg("./image/three_head_tank.png", _renderer);
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
		bossList.push_back(boss);
	}
}

bool TankBossList::checkCollisionTankBossList(Circle _boss, int k)
{
	for (int i = 0; i < bossList.size(); i++)
	{
		if (check::checkCircle_Circle(_boss, bossList[i]->getTankCircle()) == true && i != k)
			return true;
	}
	return false;
}

void TankBossList::renderList(SDL_Renderer* _renderer, SDL_Rect _camera) {
	for (int i = 0; i < bossList.size(); i++) {
		if (check::checkRect_Rect(bossList[i]->getBox(), _camera)) {
			bossList[i]->render(_renderer, _camera);
		}
	}
}

void TankBossList::handleList(MapGame _map, Circle _tankMain) {
	for (int i = 0; i < bossList.size(); i++) {
		if (!bossList[i]->getIsDestroy()) {
			bossList[i]->handleDirection();
			bool isCollisionTeam = checkCollisionTankBossList(bossList[i]->getTankCircle(), i);
			bossList[i]->handleMove(_map, _tankMain, isCollisionTeam);
		}
	}
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

				bossList[i]->setMinusHealthCurrent(_damge);
				// bossList[i]->setPercentBlood((100.0 * _damge) / bossList[i]->getTotalHealth());
				// bossList[i]->set_total_damage_wasted(_damge);
				//ds_boss[i]->set_is_damage(true);
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
