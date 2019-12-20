#include "Tile.h"

Tile::Tile(int _x, int _y, int _type) {
	box.x = _x;
	box.y = _y;
	box.w = TILE_WIDTH;
	box.h = TILE_HEIGHT;
	type = _type;
	tileClip = { 0, 0, 0, 0 };
}

Tile::~Tile() {
	;
}

void Tile::clipImg(std::string _path, SDL_Renderer* _renderer) {
	BasicObj::loadImg(_path, _renderer);
	box.w = TILE_WIDTH;
	box.h = TILE_HEIGHT;

	tileClip.w = TILE_WIDTH;
	tileClip.h = TILE_HEIGHT;
	if (type == TILE_WALL) {
		tileClip.x = 80;
		tileClip.y = 0;
	}
	else if (type == TILE_GRASS_1) {
		tileClip.x = 0;
		tileClip.y = 0;
	}
	else if (type == TILE_GRASS_2) {
		tileClip.x = 0;
		tileClip.y = 80;
	}
	else if (type == TILE_WALL_2) {
		tileClip.x = 80;
		tileClip.y = 80;
	}
}

void Tile::render(SDL_Renderer* _renderer, SDL_Rect _camera) {
	BasicObj::render(_renderer, box.x - _camera.x, box.y - _camera.y, &tileClip, 0);
}