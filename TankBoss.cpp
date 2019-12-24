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
	armor = 0;
	saveTimeShoot = 0;
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
	if (!isDestroy && check::binhPhuongKhoangCach(box.x, box.y, _tankMain.x, _tankMain.y) <= RANGE_SQUARE)
	{
		if (SDL_GetTicks() - saveTimeShoot > bulletRate) {
			Bullet* bullet = new Bullet(); // khai báo viên đạn mới
			bullet->setType(bulletType);
			bullet->loadImg(pathBulletImg, "./image/effect_shoot.png", "./image/collision3.png", _renderer);
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
			else if (rotation > PI&& rotation <= 1.5 * PI) {
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

			/* if (type_bullet == AMMO::triangle)
			{
				for (int i = 0; i < 2; i++)
				{
					AMMO* AMmo = new AMMO;
					int rotation_deviation = 0;
					if (i == 0) rotation_deviation = 20;
					else if (i == 1) rotation_deviation = -20;
					//AMmo->loadimg("image\\triangle_bullet.png", RenDer);
					bullet_order++;

					AMmo = ds_bullet_reuse[bullet_order];

					AMmo->set_time_replace_bullet(AMMO::triangle_firing_rate);
					AMmo->set_dame_bullet(AMMO::triangle_dame);
					AMmo->set_type_bullet(AMMO::triangle);

					int x = 0, y = 0;

					AMmo->set_ismove(true);
					//AMmo->load_img_hieu_ung("image\\collision3.png", RenDer);
					//AMmo->load_img_effect_shoot("image\\effect_shoot.png", RenDer);

					if (rotation + rotation_deviation >= 0 && rotation + rotation_deviation <= 90)
					{
						AMmo->set_shoot_dir(AMMO::TOP_RIGHT);
						AMmo->set_sp_x(sin(((rotation + rotation_deviation) * 3.14) / 180) * AMMO::speed_ammo);
						AMmo->set_sp_y(cos(((rotation + rotation_deviation) * 3.14) / 180) * AMMO::speed_ammo);

						x = box.x + box.w / 2 + (sin(((rotation + rotation_deviation) * 3.14) / 180) * (box.h / 2)) - AMmo->get_width() / 2;
						y = box.y + box.h / 2 - (cos(((rotation + rotation_deviation) * 3.14) / 180) * (box.h / 2)) - AMmo->get_height() / 2;
					}
					else if (rotation + rotation_deviation > 90 && rotation + rotation_deviation <= 180)
					{
						AMmo->set_shoot_dir(AMMO::BUTTON_RIGHT);
						AMmo->set_sp_x(sin(3.14 - ((rotation + rotation_deviation) * 3.14) / 180) * AMMO::speed_ammo);
						AMmo->set_sp_y(cos(3.14 - ((rotation + rotation_deviation) * 3.14) / 180) * AMMO::speed_ammo);

						x = box.x + box.w / 2 + (sin(3.14 - ((rotation + rotation_deviation) * 3.14) / 180) * (box.h / 2)) - AMmo->get_width() / 2;
						y = box.y + box.h / 2 + (cos(3.14 - ((rotation + rotation_deviation) * 3.14) / 180) * (box.h / 2)) - AMmo->get_height() / 2;
					}
					else if (rotation + rotation_deviation > 180 && rotation + rotation_deviation <= 270)
					{
						AMmo->set_shoot_dir(AMMO::BUTTON_LEFT);
						AMmo->set_sp_x(sin(((rotation + rotation_deviation) * 3.14) / 180 - 3.14) * AMMO::speed_ammo);
						AMmo->set_sp_y(cos(((rotation + rotation_deviation) * 3.14) / 180 - 3.14) * AMMO::speed_ammo);

						x = box.x + box.w / 2 - (sin(((rotation + rotation_deviation) * 3.14) / 180 - 3.14) * (box.h / 2)) - AMmo->get_width() / 2;
						y = box.y + box.h / 2 + (cos(((rotation + rotation_deviation) * 3.14) / 180 - 3.14) * (box.h / 2)) - AMmo->get_height() / 2;
					}
					else if (rotation + rotation_deviation > 270 && rotation + rotation_deviation < 360)
					{
						AMmo->set_shoot_dir(AMMO::TOP_LEFT);
						AMmo->set_sp_x(sin(6.28 - ((rotation + rotation_deviation) * 3.14) / 180) * AMMO::speed_ammo);
						AMmo->set_sp_y(cos(6.28 - ((rotation + rotation_deviation) * 3.14) / 180) * AMMO::speed_ammo);

						x = box.x + box.w / 2 - (sin(6.28 - ((rotation + rotation_deviation) * 3.14) / 180) * (box.h / 2)) - AMmo->get_width() / 2;
						y = box.y + box.h / 2 - (cos(6.28 - ((rotation + rotation_deviation) * 3.14) / 180) * (box.h / 2)) - AMmo->get_height() / 2;
					}
					AMmo->set_box_x_y(x, y);
					AMmo->set_rotation(rotation + rotation_deviation);
					AMmo->set_is_render_shoot(true);

					ds_ammo.push_back(AMmo);
				}

				}*/
				saveTimeShoot = SDL_GetTicks();
			}
		}
}

void TankBoss::handleBullet(MapGame _map, SDL_Renderer* _renderer, SDL_Rect _camera) {
	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->move();

		if (_map.checkCollitionBullet(bullets[i]->getBox())) {
			bullets[i]->setIsMove(false);
		}

		if (bullets[i]->getIsMove() == false) {
			bullets[i]->renderCollisionEffect(_renderer, _camera);
			delete bullets[i];
			bullets.erase(bullets.begin() + i);
			i--;
		}
	}
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

void TankBossList::createListBoss(MapGame _map, Circle _tankMain, int _quality, int _typeNum, SDL_Renderer* _renderer) {
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
		}
		else if (type == 2) {
			boss->setType(TankBoss::fireTank);
			boss->setBulletType(Bullet::fireBullet);
			boss->loadImg("./image/tank_boss_fire.png", _renderer);
			boss->setBulletDamge(Bullet::fireDamge);
			boss->setBulletType(Bullet::fireBullet);
			boss->setBulletRate(Bullet::fireRate);
			boss->setPathBullet("./image/danlua4.png");
		}
		else if (type == 3) {
			boss->setType(TankBoss::iceTank);
			boss->setBulletType(Bullet::iceBullet);
			boss->loadImg("./image/ice_tank.png", _renderer);
			boss->setBulletDamge(Bullet::iceDamge);
			boss->setBulletType(Bullet::iceBullet);
			boss->setBulletRate(Bullet::iceRate);
			boss->setPathBullet("./image/ice_bullet.png");
		}
		else if (type == 4) {
			boss->setType(TankBoss::triangleTank);
			boss->setBulletType(Bullet::triangleBullet);
			boss->loadImg("./image/three_head_tank.png", _renderer);
			boss->setBulletDamge(Bullet::triangleDamge);
			boss->setBulletType(Bullet::triangleBullet);
			boss->setBulletRate(Bullet::triangleRate);
			boss->setPathBullet("./image/triangle_bullet.png");
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
	for (int i = 0; i < bossList.size(); i++)
	{
		if (check::checkCircle_Circle(_boss, bossList[i]->getTankCircle()) == true && i != k)
			return true;
	}
	return false;
}

void TankBossList::renderList(SDL_Renderer* _renderer, SDL_Rect _camera) {
	for (int i = 0; i < bossList.size(); i++) {
		if (check::checkRect_Rect(bossList[i]->getBox(), _camera) && !bossList[i]->getIsDestroy()) {
			bossList[i]->render(_renderer, _camera);
		}
	}
}

void TankBossList::handleList(MapGame _map, Circle _tankMain, SDL_Renderer* _renderer, SDL_Rect _camera) {
	for (int i = 0; i < bossList.size(); i++) {
		if (!bossList[i]->getIsDestroy()) {
			bossList[i]->handleDirection();
			bossList[i]->createBullet(_renderer, _tankMain);
			bool isCollisionTeam = checkCollisionTankBossList(bossList[i]->getTankCircle(), i);
			bossList[i]->handleMove(_map, _tankMain, isCollisionTeam);
		}
		else {
			if (bossList[i]->renderDestroy(_renderer, _camera)) {
				delete bossList[i];
				bossList.erase(bossList.begin() + i);
				i--;
			}
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

void TankBossList::handleBulletOfTankList(MapGame _map, SDL_Renderer* _renderer, SDL_Rect _camera) {
	for (int i = 0; i < bossList.size(); i++) {
		bossList[i]->handleBullet(_map, _renderer, _camera);
	}
}

void TankBossList::renderBulletOfTankList(SDL_Renderer* _renderer, SDL_Rect _camera) {
	for (int i = 0; i < bossList.size(); i++) {
		bossList[i]->renderBullet(_renderer, _camera);
	}
}