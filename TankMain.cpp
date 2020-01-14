#include "TankMain.h"

TankMain::TankMain(int _x, int _y) {
	box = { _x, _y, TANK_WIDTH, TANK_HEIGHT };
	speed = 0;
	spX = spY = 0;
	rotation = 0;
	setTankCircle();
	isMouseDown = isMouseUp = false;
	saveTimeShoot = 0;
	bulletType = Bullet::BulletType::nomalBullet;
	totalHealth = healthCurrent = 0;
	bloodBar = { 0, 0, 60, 10, 100 };
	armor = 0;
	isSlowed = false;
	saveTimeIsSlowed = 0;
	isMinusHealth = false;
	isPlusHealth = false;
	damgeReceived = 0;
	slotGun1Type = slotGun2Type = Bullet::BulletType::nomalBullet;
	moneyNeedUpgradeArmor = 0;
	moneyNeedUpgradeSpeed = 0;
	moneyNeedUpgradePower = 0;
	moneyNeedUpgradeTotalHealth = 0;
	money = 0;
	totalFireBullet = totalRocketBullet = 0;
	power = 0;
	isDestroy = false;
	isChangeGun = true;
}

TankMain::~TankMain() {
	free();
}

bool TankMain::loadImg(SDL_Renderer* _renderer)
{
	if (!BasicObj::loadImg("./image/tank5.png", _renderer)) {
		return false;
	}
	if (!tamBan.loadImg("./image/tamban.png", _renderer)) {
		return false;
	}
	if (!snowFlake.loadImg("./image/snowflakes2.png", _renderer)) {
		return false;
	}
	snowFlake.setAlphaMod(191);
	frameMoney.loadImg("./image/tray_money.png", _renderer);
	frameMoney.setXY(480, 10);
	textMoney.setXY(520, 12);

	return true;
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
			bulletType = slotGun1Type;
			isChangeGun = true;
			break;
		case SDLK_2:
			bulletType = slotGun2Type;
			isChangeGun = true;
			break;
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

void TankMain::move(MapGame map, TankBossList _bossList, Circle _superTank, Circle _fireBall) {
	if (SDL_GetTicks() - saveTimeIsSlowed >= TIME_SLOWED) {
		isSlowed = false;
		box.x += spX;
		box.y += spY;
	}
	else {
		box.x += spX * 0.5;
		box.y += spY * 0.5;
	}
	setTankCircle();
	if (map.checkCollision(tankCircle) ||
		_bossList.checkCollisionTankBossList(tankCircle, -1) ||
		check::checkCircle_Circle(tankCircle, _superTank)) {
		box.x -= spX;
		setTankCircle();
	}
	if (map.checkCollision(tankCircle) ||
		_bossList.checkCollisionTankBossList(tankCircle, -1) ||
		check::checkCircle_Circle(tankCircle, _superTank)) {
		box.y -= spY;
		setTankCircle();
	}

	if (check::checkCircle_Circle(tankCircle, _fireBall)) {
		damgeReceived += 20;
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

void TankMain::render(SDL_Renderer* _renderer, SDL_Rect _camera, TTF_Font* _font) {
	// render xe tank
	BasicObj::render(_renderer, box.x - _camera.x, box.y - _camera.y, NULL, rotation);

	// render thanh máu
	bloodBar.x = box.x;
	bloodBar.y = box.y - 20;
	SDL_SetRenderDrawColor(_renderer, 0, 0, 128, 0);
	SDL_Rect rimBar = { bloodBar.x - _camera.x, bloodBar.y - _camera.y, bloodBar.width, bloodBar.height };
	SDL_RenderDrawRect(_renderer, &rimBar);
	SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 0);
	bloodBar.percent = healthCurrent * 100 / totalHealth; // tính phần trăm máu còn lại
	if (bloodBar.percent <= 0) bloodBar.percent = 0;
	SDL_Rect _bloodBar = { bloodBar.x - _camera.x + 1, bloodBar.y - _camera.y + 1, (float)(bloodBar.width - 2) * ((float)bloodBar.percent / 100), bloodBar.height - 2 };
	SDL_RenderFillRect(_renderer, &_bloodBar);

	// render minus health
	if (isMinusHealth || isPlusHealth) {
		textMinusHealth.render(_renderer, box.x + box.w - _camera.x, box.y - 20 - _camera.y, NULL, 0);
	}

	// render bông tuyết
	if (isSlowed) {
		snowFlake.render(_renderer, box.x - _camera.x, box.y - _camera.y, NULL, 0);
	}

	// render money
	frameMoney.render(_renderer, 480, 10, NULL, 0);
	textMoney.render(_renderer, 520, 12, NULL, 0);

	// render loại súng đang sử dụng
	if (isChangeGun) {
		loadTextBulletCurrent(_font, _renderer);
		isChangeGun = false;
	}
	textGunCurrent.render(_renderer, 100, 400, NULL, 0);
	if (bulletType == slotGun1Type) {
		gunSlot1.render(_renderer, 10, 400, NULL, 0);
	}
	else if (bulletType == slotGun2Type) {
		gunSlot2.render(_renderer, 10, 400, NULL, 0);
	}
}

void TankMain::handleDamgeReceived(SDL_Renderer* _renderer, TTF_Font* _font) {
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
	else {
		isMinusHealth = false;
	}
	if (healthCurrent < 0) {
		// std::cout << "Game Over" << std::endl;
		// healthCurrent = totalHealth;
		isDestroy = true;
	}
}

void TankMain::handleEatItem(std::vector<Item*> _itemlist, SDL_Renderer* _renderer, TTF_Font* _smallFont) {
	for (int i = 0; i < _itemlist.size(); i++) {
		if (check::checkRect_Circle(_itemlist[i]->getBox(), tankCircle)) {
			_itemlist[i]->setIsEat(true);
			
			if (_itemlist[i]->getType() == Item::healthItem) {
				// std::cout << "health" << std::endl;
				healthCurrent = healthCurrent + PLUS_HEALTH_ITEM < totalHealth ? healthCurrent + PLUS_HEALTH_ITEM : totalHealth;

				isPlusHealth = true;
				SDL_Color _color = { 0, 255, 0 };
				std::stringstream _plus;
				_plus.str("");
				_plus << "+ " << PLUS_HEALTH_ITEM;
				textMinusHealth.loadText(_smallFont, _plus.str(), _color, _renderer);
			}
			else if (_itemlist[i]->getType() == Item::fireBulletItem) {
				// std::cout << "fire bullet" << std::endl;
				totalFireBullet += PLUS_FIREBULLET_ITEM;
				isChangeGun = true;
				isPlusHealth = false;
			}
			else if (_itemlist[i]->getType() == Item::moneyItem) {
				// std::cout << "money" << std::endl;
				setChangeMoney(PLUS_MONEY_ITEM, _smallFont, _renderer);
				isPlusHealth = false;
			}
			return;
		}
	}
	isPlusHealth = false;
}

void TankMain::handleMoveAutomatic(int _x, int _y) {
	rotationMoveAuto = check::rotationA_B(box.x, box.y, _x, _y);
	spX = spY = 0;
	if (rotationMoveAuto >= 0 && rotationMoveAuto <= 0.5 * PI) {
		spX = (sin(rotationMoveAuto) * 5);
		spY = (cos(rotationMoveAuto) * 5);
	}
	else if (rotationMoveAuto > 0.5 * PI && rotationMoveAuto <= PI) {
		spX = (sin(PI - rotationMoveAuto) * 5);
		spY = (cos(PI - rotationMoveAuto) * 5);

	}
	else if (rotationMoveAuto > PI && rotationMoveAuto <= 1.5 * PI) {
		spX = (sin(rotationMoveAuto - PI) * 5);
		spY = (cos(rotationMoveAuto - PI) * 5);
	}
	else if (rotationMoveAuto > 1.5 * PI && rotationMoveAuto < 2 * PI) {
		spX = (sin(2 * PI - rotationMoveAuto) * 5);
		spY = (cos(2 * PI - rotationMoveAuto) * 5);
	}
	if (spY == 0) spY = 1;
	if (spX == 0) spX = 1;
	if (box.y >= _y) spY = 0;
}

bool TankMain::moveAutomatic(SDL_Renderer* _renderer, int _x, int _y) {
	if (rotationMoveAuto >= 0 && rotationMoveAuto <= 0.5 * PI) {
		box.x -= spX;
		if (box.x < _x)
			box.x += spX;
		box.y += spY;
		if (box.y + box.h > backgroundHeight - 81)
			box.y -= spY;
	}
	else if (rotationMoveAuto > 0.5 * PI && rotationMoveAuto <= PI) {
		box.x -= spX;
		if (box.x < _x)
			box.x += spX;
		box.y -= spY;
		if (box.y + box.h > backgroundHeight - 81)
			box.y += spY;
	}
	else if (rotationMoveAuto > PI && rotationMoveAuto <= 1.5 * PI) {
		box.x += spX;
		if (box.x > _x)
			box.x -= spX;
		box.y -= spY;
		if (box.y + box.h > backgroundHeight - 81)
			box.y += spY;
	}
	else if (rotationMoveAuto > 1.5 * PI && rotationMoveAuto < 2 * PI) {
		box.x += spX;
		if (box.x > _x)
			box.x -= spX;
		box.y += spY;
		if (box.y + box.h > backgroundHeight - 81)
			box.y -= spY;
	}

	if (box.y >= _y){
		if (rotationMoveAuto >= 0 && rotationMoveAuto <= PI) {
			if (box.x <= _x + 5)
				return true;
		}
		else {
			if (box.x >= _x - 5)
				return true;
		}
	}
	return false;
}

void TankMain::setChangeMoney(int _change, TTF_Font* _font, SDL_Renderer* _renderer) {
	if (_change > 0) {
		money += _change;
		SDL_Color _color = { 0, 0, 128 };
		std::stringstream _money;
		_money.str("");
		_money << money;
		textMoney.loadText(_font, _money.str(), _color, _renderer);
	}
}

void TankMain::setMoney(int _money, TTF_Font* _font, SDL_Renderer* _renderer) {
	money = _money;
	SDL_Color _color = { 0, 0, 128 };
	std::stringstream _moneyText;
	_moneyText.str("");
	_moneyText << money;
	textMoney.loadText(_font, _moneyText.str(), _color, _renderer);
}

void TankMain::assign(SDL_Renderer* _renderer) {
	healthCurrent = totalHealth;
	spX = spY = 0;
	rotation = 0;
	box.x = box.y = 100;
	setTankCircle();
	isMouseDown = isMouseUp = false;
	saveTimeShoot = 0;
	bulletType = slotGun1Type;
	bloodBar = { 0, 0, 60, 10, 100 };
	isSlowed = false;
	saveTimeIsSlowed = 0;
	isMinusHealth = false;
	isPlusHealth = false;
	damgeReceived = 0;
	isDestroy = false;
	isChangeGun = true;
	loadGun12(_renderer);
}

void TankMain::loadGun12(SDL_Renderer* _renderer) {
	if (slotGun1Type == Bullet::nomalBullet) {
		gunSlot1.loadImg("./image/TD_gun.png", _renderer);
	}
	else if (slotGun1Type == Bullet::fireBullet) {
		gunSlot1.loadImg("./image/fire_gun.png", _renderer);
	}
	else if (slotGun1Type == Bullet::rocketBullet) {
		gunSlot1.loadImg("./image/rocket_gun.png", _renderer);
	}

	if (slotGun2Type == Bullet::nomalBullet) {
		gunSlot2.loadImg("./image/TD_gun.png", _renderer);
	}
	else if (slotGun2Type == Bullet::fireBullet) {
		gunSlot2.loadImg("./image/fire_gun.png", _renderer);
	}
	else if (slotGun2Type == Bullet::rocketBullet) {
		gunSlot2.loadImg("./image/rocket_gun.png", _renderer);
	}
}

void TankMain::renderTam(SDL_Renderer* _renderer) {
	int mouseX = 0, mouseY = 0;
	SDL_GetMouseState(&mouseX, &mouseY);
	tamBan.render(_renderer, mouseX - tamBan.getW() / 2, mouseY - tamBan.getH() / 2, NULL, 0);
}

void TankMain::createBullet(SDL_Renderer* _renderer, Mix_Chunk* _mixChuck[]) {
	if (isMouseDown && !isMouseUp) { // kiểm tra chuột trái có đang được nhấn
		Bullet::BulletFirtingRate firtingRate{};

		// Kiểm tra tốc độ bắn của loại đạn đang sử dung
		if (bulletType == Bullet::nomalBullet) {
			firtingRate = Bullet::nomalRate;
		}
		else if (bulletType == Bullet::fireBullet) {
			if (totalFireBullet <= 0) {
				Mix_PlayChannel(-1, _mixChuck[SOUND_NULL_BULLET], 0);
				return;
			}
			firtingRate = Bullet::fireRate;
		}
		else if (bulletType == Bullet::rocketBullet) {
			if (totalRocketBullet <= 0) {
				Mix_PlayChannel(-1, _mixChuck[SOUND_NULL_BULLET], 0);
				return;
			}
			firtingRate = Bullet::rocketRate;
		}

		// kiểm tra thời điểm bắn đạn trước so với thời điểm bắn đạn sau có lớn hơn tốc độ bắn hay không
		if (SDL_GetTicks() - saveTimeShoot > firtingRate) {
			std::string _imgBullet; // hình viên đạn
			int _damge = Bullet::nomalDamge; // damge viên đạn
			if (bulletType == Bullet::nomalBullet) {
				_imgBullet = "./image/ammo.png";
				_damge = Bullet::nomalDamge;
				Mix_PlayChannel(-1, _mixChuck[SOUND_SHOOT_NORMAL], 0);
			}
			else if (bulletType == Bullet::fireBullet) {
				_imgBullet = "./image/danlua4.png";
				_damge = Bullet::fireDamge;
				isChangeGun = true;
				totalFireBullet--;
				Mix_PlayChannel(-1, _mixChuck[SOUND_SHOOT_FIRE], 0);
			}
			else if (bulletType == Bullet::rocketBullet) {
				_imgBullet = "./image/bullet_rocket.png";
				_damge = Bullet::rocketDamge;
				isChangeGun = true;
				totalRocketBullet--;
				Mix_PlayChannel(-1, _mixChuck[SOUND_SHOOT_ROCKET], 0);
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
			bullet->setDamge(_damge * power);
			bullet->setIsMove(true);

			bullets.push_back(bullet);
			saveTimeShoot = SDL_GetTicks();
		}
	}
}

void TankMain::handleBullet(MapGame _map, SDL_Renderer* _renderer, SDL_Rect _camera, TankBossList _tankList, SuperTankBoss* _superTank, Mix_Chunk* _mixChuck[]) {
	for (int i = 0; i < bullets.size(); i++) {		
		bullets[i]->move();

		if (_map.checkCollisionRect(bullets[i]->getBox())) {
			bullets[i]->setIsMove(false);
			Mix_PlayChannel(-1, _mixChuck[SOUND_COLLISION_WALL], 0);
		}
		else if (_tankList.checkCollisionBullet(bullets[i]->getBox(), true, bullets[i]->getDamge())) {
			bullets[i]->setIsMove(false);
			Mix_PlayChannel(-1, _mixChuck[SOUND_COLLISION_ENEMIES], 0);
		}
		else if (_superTank != NULL) {
			if (check::checkRect_Circle(bullets[i]->getBox(), _superTank->getTankCircle())) {
				_superTank->setDamgeReceived(bullets[i]->getDamge());
				bullets[i]->setIsMove(false);
				Mix_PlayChannel(-1, _mixChuck[SOUND_COLLISION_ENEMIES], 0);
			}
		}
		if (bullets[i]->getIsMove() == false) {
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

void TankMain::loadTextBulletCurrent(TTF_Font* _font, SDL_Renderer* _renderer) {
	SDL_Color color = { 150, 0, 0 };
	std::stringstream t;
	t.str("");
	if (bulletType == Bullet::nomalBullet)
		t << "oo";
	else if (bulletType == Bullet::fireBullet)
		t << totalFireBullet;
	else if (bulletType == Bullet::rocketBullet)
		t << totalRocketBullet;
	textGunCurrent.loadText(_font, t.str(), color, _renderer);
}
