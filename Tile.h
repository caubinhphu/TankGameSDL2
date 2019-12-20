#pragma once
#ifndef TILE_H_
#define TILE_H_

#include "BasicObject.h"

#define TILE_WIDTH 80
#define TILE_HEIGHT 80

#define TOTAL_TYPE_TILE 4
#define TILE_WALL 0
#define TILE_GRASS_1 1
#define TILE_GRASS_2 2
#define TILE_WALL_2 3

class Tile : public BasicObj {
private:
	int type;
	//std::vector<SDL_Rect> tiles;
	SDL_Rect tileClip;
public:
	Tile(int _x, int _y, int _type);
	~Tile();
	void clipImg(std::string _path, SDL_Renderer* _renderer);
	void render(SDL_Renderer* _renderer, SDL_Rect _camera);
	
	int getType() { return type; };
	void setType(int _type) { type = _type; };
};


#endif // !TILE_H_
