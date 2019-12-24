﻿#include "general.h"
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

	return true;
}

bool load() {
	bool res = true;
	if (!tank.loadImg("./image/tank5.png", renderer)) {
		res = false;
	}
	if (!tank.loadTamBan("./image/tamban.png", renderer)) {
		res = false;
	}
	if (!map.loadMap("./image/mapimg5.png", "./general/mapgame.map", renderer)) {
		res = false;
	}
	return res;
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
		if (load()) {
			SDL_Rect camera = { 0, 0, cameraWidth, cameraHeight }; // khai báo camera
			SDL_ShowCursor(SDL_DISABLE); // ẩn con trỏ chuột
			bossList.createListBoss(map, tank.getTankCircle(), 50, 4, renderer);
			while (!out) {
				while (SDL_PollEvent(&event) != 0) { // bắt các sự kiện
					if (event.type == SDL_QUIT) {
						out = true;
					}
					tank.handleEvents(&event, camera);
				}

				SDL_RenderClear(renderer); // clear màn hình render
				SDL_SetRenderDrawColor(renderer, 100, 50, 0, 0);
				map.render(renderer, camera);
				tank.move(map, bossList);
				tank.setCamera(camera);
				bossList.handleList(map, tank.getTankCircle(), renderer, camera);
				tank.createBullet(renderer);
				bossList.handleBulletOfTankList(map, renderer, camera);
				tank.handleBullet(map, renderer, camera, bossList);
				tank.renderBullet(renderer, camera);
				bossList.renderBulletOfTankList(renderer, camera);
				tank.render(renderer, camera);
				bossList.renderList(renderer, camera);
				bossList.renderList(renderer, camera);
				tank.renderTam(renderer);
				SDL_RenderPresent(renderer);
			}
			
		}
		close();
	}

	return 0;
}