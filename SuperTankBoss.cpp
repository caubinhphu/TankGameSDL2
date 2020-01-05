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
	for (int i = 0; i < TOTAL_TILE; i++)
	{
		int hang = 0, cot = 0;
		if (_mapWall[i] == 1)
		{
			hang = i / 20;
			cot = i % 20;

			if (hang != 0 && hang != 19 && cot != 0 && cot != 19)
			{
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
	//if (frame + 1 < 0)
	//{
	//	frame++;
	//	return false;
	//}
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
	else{
		spX = spY = 0;
	}
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
	return true;
}

void SuperTankBoss::render(SDL_Renderer* _renderer, SDL_Rect _camera) {
	// render tank
	BasicObj::render(_renderer, box.x - _camera.x, box.y - _camera.y, NULL, rotation);

	// render vòng lửa
	ballFire.render(_renderer, box.x - 45 - _camera.x, box.y - 45 - _camera.y, NULL, 0);

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

void SuperTankBoss::setCircleBallFire(int _x, int _y) {
	ballFireCircle.x = _x + ballFire.getW() / 2;
	ballFireCircle.y = _y + ballFire.getH() / 2;
	ballFireCircle.r = ballFire.getW() / 2;
}
