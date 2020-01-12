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

bool MapGame::checkCollisionRect(SDL_Rect _box) {
	for (int i = 0; i < tiles.size(); i++) {
		if (tiles[i]->getType() == TILE_WALL) {
			if (check::checkRect_Rect(tiles[i]->getBox(), _box)) {
				return true;
			}
		}
	}
	return false;
}

std::vector<int> MapGame::getMapWallDigital() {
	std::vector<int> _map(TOTAL_TILE, 0);
	for (int i = 0; i < TOTAL_TILE; i++) {
		if (tiles[i]->getType() == TILE_WALL)
			_map[i] = 1;
	}
	return _map;
}

void MapGame::reload() {
	std::vector<int> mapWall = getMapWallDigital();
	for (int i = 0; i < mapWall.size(); i++) {
		if (mapWall[i] == 1) {
			if (i / 20 != 0 && i / 20 != 19 && i % 20 != 0 && i % 20 != 19) {
				tiles[i]->setType(TILE_WALL_2);
				tiles[i]->setTileClip({ TILE_WIDTH, TILE_HEIGHT, 80, 80 });
			}
		}
	}
}