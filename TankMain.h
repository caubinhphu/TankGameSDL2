#pragma once

#ifndef TANKMAIN_H_
#define TANKMAIN_H_

#include "BasicObject.h"
#include "MapGame.h"
#include "Bullet.h"
#include "TankBoss.h"
#include "Item.h"
#include "SuperTankBoss.h"

#define TANK_WIDTH 60
#define TANK_HEIGHT 60
#define TIME_SLOWED 500

#define PLUS_UPGRADE_ARMOR 1
#define PLUS_UPGRADE_SPEED 1
#define PLUS_UPGRADE_POWER 1
#define PLUS_UPGRADE_HEALTH 10

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

	BasicObj snowFlake;
	bool isSlowed;
	Uint32 saveTimeIsSlowed;

	BasicObj textMinusHealth;
	bool isMinusHealth;
	bool isPlusHealth;
	double rotationMoveAuto;
	int damgeReceived;

	Bullet::BulletType slotGun1Type, slotGun2Type;
	int moneyNeedUpgradeArmor;
	int moneyNeedUpgradeSpeed;
	int moneyNeedUpgradePower;
	int moneyNeedUpgradeTotalHealth;
	int money;
	int totalFireBullet;
	int totalRocketBullet;

	bool isHaveFireGun, isHaveRocketGun;
	bool isDestroy;

	BasicObj frameMoney;
	BasicObj textMoney;
	BasicObj gunSlot1, gunSlot2;
	BasicObj textGunCurrent;
	bool isChangeGun;

	BasicObj destroyImg[4];
	int frameDestroy;
public:
	TankMain(int _x, int _y);
	~TankMain();

	bool loadImg(SDL_Renderer* _renderer);

	void renderTam(SDL_Renderer* _renderer);

	void handleEvents(SDL_Event* _e, SDL_Rect _camera);
	void createBullet(SDL_Renderer* _renderer, Mix_Chunk* _mixChuck[]);
	void handleBullet(MapGame _map, SDL_Renderer* _renderer, SDL_Rect _camera, TankBossList _tankList, SuperTankBoss* _superTank, Mix_Chunk* _mixChuck[]);
	void renderBullet(SDL_Renderer* _renderer, SDL_Rect _camera);
	void loadTextBulletCurrent(TTF_Font* _font, SDL_Renderer* _renderer);
	void loadDestroyImg(SDL_Renderer* _renderer);
	bool renderDestroy(SDL_Renderer* _renderer, SDL_Rect _camera);

	void move(MapGame map, TankBossList _bossList, Circle _superTank, Circle _fireBall);
	void setCamera(SDL_Rect &_camera);
	void render(SDL_Renderer* _renderer, SDL_Rect _camera, TTF_Font* _font);

	int getSpeed() { return speed; };
	void setPlusSpeed(int _plus) { speed += _plus; };
	void setSpeed(int _speed) { speed = _speed; };

	void setSpXY(int _spX, int _spY) { spX = _spX; spY = _spY; };
	Circle getTankCircle() { return tankCircle; };

	void setPlusHealthCurrent(int _healthPlus) { healthCurrent += _healthPlus; };
	int getHealthCurrent() { return healthCurrent; };
	// void setDamageReceived(int _damgeReceived, SDL_Renderer* _renderer, TTF_Font* _font);
	void setDamgeReceived(int _damgeReceived) { damgeReceived += _damgeReceived; };
	void handleDamgeReceived(SDL_Renderer* _renderer, TTF_Font* _font, SDL_Rect _camera);
	void setIsSlowed(bool _is) { isSlowed = _is; };
	void setSaveTimeIsSlowed(Uint32 _time) { saveTimeIsSlowed = _time; };
	void handleEatItem(std::vector<Item*> _itemlist, SDL_Renderer* _renderer, TTF_Font* _smallFont, Mix_Chunk* _mixChuck[]);

	void handleMoveAutomatic(int _x, int _y);
	bool moveAutomatic(SDL_Renderer* _renderer, int _x, int _y);

	void setPlusArmor(int _plus) { armor += _plus; };
	int getArmor() { return armor; };
	void setArmor(int _armor) { armor = _armor; };

	void setPlusPower(int _plus) { power += _plus; };
	int getPower() { return power; };
	void setPower(int _power) { power = _power; };

	void setPlusTotalHealth(int _plus) { totalHealth += _plus; healthCurrent = totalHealth; };
	int getTotalHealth() { return totalHealth; };
	void setTotalHealth(int _total) { totalHealth = _total; healthCurrent = totalHealth; };

	Bullet::BulletType getSlotGun1Type() { return slotGun1Type; }
	Bullet::BulletType getSlotGun2Type() { return slotGun2Type; }
	void setSlotGun1Type(Bullet::BulletType _type) { slotGun1Type = _type; bulletType = _type; }
	void setSlotGun2Type(Bullet::BulletType _type) { slotGun2Type = _type; }

	int getMoneyNeedUpgradeArmor() { return moneyNeedUpgradeArmor; }
	void setMoneyNeedUpgradeArmor() { moneyNeedUpgradeArmor *= 2; }
 	void setTotalMoneyNeedUpgradeArmor(int _m) { moneyNeedUpgradeArmor = _m; }

	int getMoneyNeedUpgradeSpeed() { return moneyNeedUpgradeSpeed; }
	void setMoneyNeedUpgradeSpeed() { moneyNeedUpgradeSpeed *= 8; }
	void setTotalMoneyNeedUpgradeSpeed(const int _m) { moneyNeedUpgradeSpeed = _m; }

	int getMoneyNeedUpgradePower() { return moneyNeedUpgradePower; }
	void setMoneyNeedUpgradePower() { moneyNeedUpgradePower *= 5; }
	void setTotalMoneyNeedUpgradePower(const int _m) { moneyNeedUpgradePower = _m; }

	int getMoneyNeedUpgradeTotalHealth() { return moneyNeedUpgradeTotalHealth; }
	void setMoneyNeedUpgradeTotalHealth() { moneyNeedUpgradeTotalHealth *= 2; }
	void setTotalMoneyNeedUpgradeTotalHealth(const int _m) { moneyNeedUpgradeTotalHealth = _m; }

	int getMoney() { return money; };
	void setChangeMoney(int _change, TTF_Font* _font, SDL_Renderer* _renderer);
	void setMoney(int _money, TTF_Font* _font, SDL_Renderer* _renderer);

	int getTotalFireBullet() { return totalFireBullet; };
	void setTotalFireBullet(int _total) { totalFireBullet = _total; };

	int getTotalRocketBullet() { return totalRocketBullet; };
	void setPlusTotalRocketBullet(int _plus) { totalRocketBullet += _plus; };

	void setIsHaveFireGun(bool _have) { isHaveFireGun = _have; }
	bool getIsHaveFireGun() { return isHaveFireGun; }
	void setIsHaveRocketGun(bool _have) { isHaveRocketGun = _have; }
	bool getIsHaveRocketGun() { return isHaveRocketGun; }

	bool getIsDestroy() { return isDestroy; };

	void assign(SDL_Renderer* _renderer);
	void loadGun12(SDL_Renderer* _renderer);
};

#endif // !TANKMAIN_H_
