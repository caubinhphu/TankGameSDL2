#include "general.h"
#include "TankMain.h"
#include "MapGame.h";
#include "TankBoss.h"

MapGame map;
TankMain tank(100, 100);
TankBossList bossList;

bool init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		return false;
	}
	window = SDL_CreateWindow("TANK GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, cameraWidth, cameraHeight, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		return false;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		return false;
	}
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		return false;
	}
#ifdef _SDL_TTF_H
	if (TTF_Init() == -1)
		return false;
#endif
	return true;
}

bool load() {
	if (!tank.loadImg(renderer)) {
		return false;
	}
	if (!map.loadMap("./image/mapimg5.png", "./general/mapgame.map", renderer)) {
		return false;
	}

	// load font
	font = TTF_OpenFont("./font/turok.ttf", 30);
	smallFont = TTF_OpenFont("./font/turok.ttf", 25);
	bigFont = TTF_OpenFont("./font/turok.ttf", 50);

	return true;
}

void close() {
	tank.free();
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
}

int main(int arc, char* arg[]) {

	if (init()) {
		srand(time(NULL));
		bool out = false;
		int level = 1;
		bool isLevelUp = false;
		if (load()) {
			SDL_Rect camera = { 0, 0, cameraWidth, cameraHeight }; // khai báo camera
			SDL_ShowCursor(SDL_DISABLE); // ẩn con trỏ chuột
			bossList.createListBoss(map, tank.getTankCircle(), level * 5, level < 5 ? level : 4, renderer, level * 100, level * 5);
			while (!out) {
				while (SDL_PollEvent(&event) != 0) { // bắt các sự kiện
					if (event.type == SDL_QUIT) {
						out = true;
					}
					tank.handleEvents(&event, camera);
				}

				if (isLevelUp) { // tăng độ level game
					level++;
					bossList.createListBoss(map, tank.getTankCircle(), level * 5, level < 5 ? level : 4, renderer, level * 100, level * 5);
					isLevelUp = false;
					std::cout << "level up" << std::endl;
				}

				SDL_RenderClear(renderer); // clear màn hình render
				SDL_SetRenderDrawColor(renderer, 100, 50, 0, 0);
				map.render(renderer, camera);
				tank.move(map, bossList);
				tank.setCamera(camera);
				bossList.handleList(map, tank.getTankCircle(), renderer, camera);
				tank.createBullet(renderer);
				bool _isSlowedTankMain = false;
				tank.setDamageReceived(bossList.handleBulletOfTankList(map, renderer, camera, tank.getTankCircle(), _isSlowedTankMain), renderer, smallFont);
				if (_isSlowedTankMain) {
					tank.setIsSlowed(true);
					tank.setSaveTimeIsSlowed(SDL_GetTicks());
				}
				tank.handleBullet(map, renderer, camera, bossList);

				if (bossList.getQualityBoss() == 0) {
					isLevelUp = true;
				}

				tank.renderBullet(renderer, camera);
				bossList.renderBulletOfTankList(renderer, camera);
				tank.render(renderer, camera);
				bossList.renderList(renderer, camera, smallFont);
				tank.renderTam(renderer);
				SDL_RenderPresent(renderer);
			}
			
		}
		close();
	}

	return 0;
}