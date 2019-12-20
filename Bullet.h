#pragma once
#ifndef BULLET_H_
#define BULLET_H_

#include "BasicObject.h"

class Bullet : public BasicObj {
public:
	enum ShootDir
	{
		TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT,
	};
	static const int speed = 20;
private:
	int spX, spY;
	double rotation;
	bool isMove;
	ShootDir dir;
public:
	Bullet();
	~Bullet();
	void move();
	
	void setSpX(int _spX) { spX = _spX; };
	void setSpY(int _spY) { spY = _spY; };
	bool getIsMove() { return isMove; };
	void setIsMove(bool _isMove) { isMove = _isMove; };
	void setDir(ShootDir _dir) { dir = _dir; };
	ShootDir getDir() { return dir; };
	void setRotation(double _rotation) { rotation = _rotation; };
	void render(SDL_Renderer* _renderer, SDL_Rect _camera);
};

#endif // !BULLET_H_
