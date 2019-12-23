#include "MapGame.h"

MapGame::MapGame() {
	;
}

MapGame::~MapGame() {
	;
}

bool MapGame::loadMap(std::string _pathImg, std::string _pathMapText, SDL_Renderer* _renderer) {
	std::ifstream map(_pathMapText.c_str());
	if (map.fail()) {
		return false;
	}
	int type = -1, x = 0, y = 0;
	for (int i = 0; i < TOTAL_TILE; i++) {
		Tile* tmpTile = new Tile(0, 0, -1);
		map >> type;
		if (map.fail()) {
			map.close();
			std::cout << "fail";
			return false;
		}
		tmpTile->setXY(x, y);
		tmpTile->setType(type);
		tmpTile->clipImg(_pathImg, _renderer);
		tiles.push_back(tmpTile);
		x += TILE_WIDTH;
		if (x >= backgroundWidth) {
			x = 0;
			y += TILE_HEIGHT;
		}
	}
	map.close();
	return true;
}

void MapGame::render(SDL_Renderer* _renderer, SDL_Rect _camera) {
	for (int i = 0; i < TOTAL_TILE; i++) {
		if (check::checkRect_Rect(tiles[i]->getBox(), _camera))
			tiles[i]->render(_renderer, _camera);
	}
}

bool MapGame::checkCollision(Circle tankMain) {
	for (int i = 0; i < TOTAL_TILE; i++) {
		if (tiles[i]->getType() == TILE_WALL) {
			if (check::checkRect_Circle(tiles[i]->getBox(), tankMain)) {
				return true;
			}
		}
	}
	return false;
}

bool MapGame::checkCollitionBullet(SDL_Rect _bullet) {
	for (int i = 0; i < tiles.size(); i++) {
		if (tiles[i]->getType() == TILE_WALL) {
			if (check::checkRect_Rect(tiles[i]->getBox(), _bullet)) {
				return true;
			}
		}
	}
	return false;
}

bool MapGame::checkCollisionTankBoss(SDL_Rect _boxTank) {
	for (int i = 0; i < TOTAL_TILE; i++) {
		if (tiles[i]->getType() == TILE_WALL) {
			if (check::checkRect_Rect(tiles[i]->getBox(), _boxTank)) {
				return true;
			}
		}
	}
	return false;
}