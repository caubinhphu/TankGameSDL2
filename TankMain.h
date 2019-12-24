#pragma once

#ifndef TANKMAIN_H_
#define TANKMAIN_H_

#include "BasicObject.h"
#include "MapGame.h"
#include "Bullet.h"
#include "TankBoss.h"

#define TANK_WIDTH 60
#define TANK_HEIGHT 60

class TankMain : public BasicObj {
private:
	int speed; // tốc độ của xe
	double rotation; // góc quay của xe (radian)
	int spX, spY; // gia tốc
	BasicObj tamBan; // tâm ngắm bắn
	Circle tankCircle;
	void setTankCircle();
	std::vector<Bullet*> bullets;
	bool isMouseDown, isMouseUp;
	Uint32 saveTimeShoot;
	Bullet::BulletType bulletType;

	BloodBar bloodBar;//mau
	int armor;//giap
	int totalHealth;//tong mau
	int healthCurrent;
	int power;

public:
	TankMain(int _x, int _y);
	~TankMain();

	bool loadTamBan(std::string _path, SDL_Renderer* _renderer);
	void renderTam(SDL_Renderer* _renderer);

	void handleEvents(SDL_Event* _e, SDL_Rect _camera);
	void createBullet(SDL_Renderer* _renderer);
	void handleBullet(MapGame _map, SDL_Renderer* _renderer, SDL_Rect _camera, TankBossList _tankList);
	void renderBullet(SDL_Renderer* _renderer, SDL_Rect _camera);
	void move(MapGame map, TankBossList _bossList);
	void setCamera(SDL_Rect &_camera);
	void render(SDL_Renderer* _renderer, SDL_Rect _camera);

	int getSpeed() { return speed; };
	void setSpeed(int _speed) { speed = _speed; };

	Circle getTankCircle() { return tankCircle; };

	void setPlusAmor(int _armorPlus) { armor += _armorPlus; };
	void setPlusHealthCurrent(int _healthPlus) { healthCurrent += _healthPlus; };
	int getHealthCurrent() { return healthCurrent; };
	void setPlusTotalHealth(int _totalPlus) { totalHealth += _totalPlus; };
	void setPlusPower(int _powerPlus) { power += _powerPlus; };
	void setDamageReceived(int _dam_damgeReceivedge);
};

#endif // !TANKMAIN_H_
