#pragma once
#ifndef BULLET_H_
#define BULLET_H_

#include "BasicObject.h"

class Bullet : public BasicObj {
public:
	enum ShootDir {
		TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT,
	};
	enum BulletType {
		nomalBullet, fireBullet, rocketBullet, fireBossBullet, ballBullet, ballNomalBullet, iceBullet, triangleBullet,
		marblesLv1Bullet, marblesLv2Bullet, marblesLv3Bullet, 
	};
	enum BulletFirtingRate {
		nomalRate= 300,
		fireRate = 600,
		rocketRate = 1000,
		fireBossRate = 500,
		ballBossRate = 700,
		ballNormalBoosRate = 600,
		iceRate = 600,
		triangleRate = 500,
		marblesRate = 700,
	};
	enum DameBullet
	{
		normalDame = 10,
		fireDame = 20,
		fireBossDame = 50,
		ballBossDame = 40,
		ballNormalBossDame = 40,
		redzoneDame = 50,
		iceDame = 10,
		triangleDame = 35,
		marblesDame = 45,
		rocketDame = 200,
	};

	static const int speed = 20; // 20
private:
	int spX, spY;
	double rotation;
	bool isMove;
	ShootDir dir;
	BasicObj shootEffect;
	BasicObj collisionEffect;
	bool isEffectShoot;
	BulletType type;
	BulletFirtingRate firtingRate;
public:
	Bullet();
	~Bullet();
	void move();
	bool loadImg(std::string _pathBullet, std::string _pathShootEffect, std::string _pathCollisionEffect, SDL_Renderer* _renderer);
	void setSpX(int _spX) { spX = _spX; };
	void setSpY(int _spY) { spY = _spY; };
	bool getIsMove() { return isMove; };
	void setIsMove(bool _isMove) { isMove = _isMove; };
	void setDir(ShootDir _dir) { dir = _dir; };
	ShootDir getDir() { return dir; };
	void setRotation(double _rotation) { rotation = _rotation; };
	void render(SDL_Renderer* _renderer, SDL_Rect _camera);
	void setIsEffectShoot(bool _is) { isEffectShoot = _is; };
	bool getIsEffectShoot() { return isEffectShoot; };
	void renderShootEffect(SDL_Renderer* _renderer, SDL_Rect _camera, SDL_Rect _boxTankMain);
	void renderCollisionEffect(SDL_Renderer* _renderer, SDL_Rect _camera);
	BulletType getType() { return type; };
	void setType(BulletType _type) { type = _type; };
	BulletFirtingRate getFirtingRate() { return firtingRate; };
};

#endif // !BULLET_H_
