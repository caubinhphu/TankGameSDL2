#pragma once
#ifndef MAPGAME_H_
#define MAPGAME_H_

#include "Tile.h"

#define TOTAL_TILE 400

class MapGame {
private:
	std::vector<Tile*> tiles;
public:
	MapGame();
	~MapGame();
	bool loadMap(std::string _pathImg, std::string _pathMapText, SDL_Renderer* _renderer);
	void render(SDL_Renderer* _renderer, SDL_Rect _camera);
	bool checkCollision(Circle tankMain);
	//bool checkCollitionBullet(SDL_Rect _bullet);
	//bool checkCollisionTankBoss(SDL_Rect _boxTank);
	bool checkCollisionRect(SDL_Rect _box);
};

#endif // !MAPGAME_H_
