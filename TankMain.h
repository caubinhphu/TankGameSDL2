#pragma once
#include "BasicObject.h"
#include "MapGame.h"

#ifndef TANKMAIN_H_
#define TANKMAIN_H_

class TankMain : public BasicObj {
private:
	int speed; // tốc độ của xe
	double rotation; // góc quay của xe
	int spX, spY; // gia tốc
	BasicObj tamBan; // tâm ngắm bắn
	Circle tankCircle;
	void setTankCircle();
public:
	TankMain(int _x, int _y);
	~TankMain();

	bool loadTamBan(std::string _path, SDL_Renderer* _renderer);
	void renderTam(SDL_Renderer* _renderer);

	void handleEvents(SDL_Event* _e, SDL_Rect _camera);
	void move(MapGame map);
	void setCamera(SDL_Rect &_camera);
	void render(SDL_Renderer* _renderer, SDL_Rect _camera);

	int getSpeed() { return speed; };
	void setSpeed(int s) { speed = s; };

	Circle getTankCircle() { return tankCircle; };
};

#endif // !TANKMAIN_H_
