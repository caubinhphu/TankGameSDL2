#include "general.h"
#include "TankMain.h"
#include "MapGame.h";
#include "TankBoss.h"
#include "Item.h"
#include "SuperTankBoss.h"

#define TOTAL_LEVEL_GAME 2

MapGame map;
TankMain tank(100, 100);
TankBossList bossList;
ItemList itemList;
SuperTankBoss* superTankBoss;


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
		bool isLevelUp = false; // level up?
		bool isLoadedSuperTank = false; // đã load super tank?
		bool isSuperTankAppear = false; // super tank xuất hiện?
		bool isAllowRenderSuperTank = false; // cho phép render super tank?
		bool isAllowRenderEffectSuperTank = false; // cho phép render hiện ứng xuất hiện super tank?
		bool isAllowSuperTankMove = false;
		bool isAllowHandleAppearSuperTank = false; // cho phép xử lý sau khi tường nổ?
		bool isAllowTankMainMove = true; // cho phép tank main di chuyển?
		bool isAllowCreateTankBossList = false; // cho phép tạo bost list?
		if (load()) {
			SDL_Rect camera = { 0, 0, cameraWidth, cameraHeight }; // khai báo camera
			SDL_ShowCursor(SDL_DISABLE); // ẩn con trỏ chuột
			bossList.createListBoss(map, tank.getTankCircle(), level, level < 5 ? level : 4, renderer, level * 50, level * 2);
			while (!out) {
				while (SDL_PollEvent(&event) != 0) { // bắt các sự kiện
					if (event.type == SDL_QUIT) {
						out = true;
					}
					if (isAllowTankMainMove)
						tank.handleEvents(&event, camera);

					if (event.type == SDL_KEYDOWN) {
						if (event.key.keysym.sym == SDLK_p) {
							tank.handleMoveAutomatic(backgroundWidth / 2 - tank.getW() / 2, backgroundHeight - 90 - tank.getH());
							isAllowTankMainMove = false;
						}
						else if (event.key.keysym.sym == SDLK_o) {
							isAllowTankMainMove = true;
						}
					}

				}

				if (isLevelUp) { // tăng độ level game
					level++;
					isLevelUp = false;
					std::cout << "level up" << std::endl;
					isAllowCreateTankBossList = true;
				}

				//if (level >= 1 && SDL_GetTicks() - itemList.getSaveTimeCreate() >= TIME_RECREATE_ITEM) {
				//	itemList.createList(renderer, map);
				//}

				SDL_RenderClear(renderer); // clear màn hình render
				SDL_SetRenderDrawColor(renderer, 100, 50, 0, 0);
				map.render(renderer, camera);
				if (level == TOTAL_LEVEL_GAME && !isLoadedSuperTank) {
					SuperTankBoss* _superTank = new SuperTankBoss;

					_superTank->loadImg(renderer);
					_superTank->loadEffectAppear(renderer);
					_superTank->handleEffectAppear(map.getMapWallDigital());
				//	_superTank->load_ball_fire(renderer, "image\\ball_fire_3.png");
				//	_superTank->load_ds_bullet_reuse(renderer);
				//	_superTank->load_img_shield(renderer, "image\\shield.png");
				//	_superTank->set_is_destroy(false);
					isLoadedSuperTank = true;
					isAllowRenderEffectSuperTank = true;
					isAllowCreateTankBossList = false;
					isSuperTankAppear = true;
					/*BossEs.set_flag_tank_boss(false);
					BossEs.set_level(99);*/
					superTankBoss = _superTank;
				}
				if (isAllowRenderEffectSuperTank) {
					if (superTankBoss->renderEffectAppear(renderer, camera))
					{
						isAllowRenderEffectSuperTank = false;
						isAllowHandleAppearSuperTank = true;
						superTankBoss->freePositionWallTileList();
					}
				}
				if (isAllowHandleAppearSuperTank && !isAllowRenderEffectSuperTank) {
					map.reload();
					map.render(renderer, camera);
					isAllowTankMainMove = false;

					tank.handleMoveAutomatic(backgroundWidth / 2 - tank.getW() / 2, backgroundHeight - 90 - tank.getH());
					// tank.setXY(backgroundWidth / 2 - tank.getW() / 2, backgroundHeight - 82 - tank.getH());
					superTankBoss->setXY(backgroundWidth / 2 - superTankBoss->getW() / 2, backgroundHeight - 2 * cameraHeight);
					superTankBoss->setTankCircle(superTankBoss->getX(), superTankBoss->getY());
					superTankBoss->handleMoveAppear();
					isAllowHandleAppearSuperTank = false;
					//is_appear_boss = true;
				}
				if (!isAllowTankMainMove) {
					if (tank.moveAutomatic(renderer, backgroundWidth / 2 - tank.getW() / 2, backgroundHeight - 90 - tank.getH())) {
						tank.setSpXY(0, 0);
						isAllowRenderSuperTank = true;
					}
				}

				if (isSuperTankAppear && isAllowRenderSuperTank)
				{
					if (superTankBoss->moveAuto(backgroundHeight - cameraHeight + 20))
					{
						isAllowTankMainMove = true;
						isSuperTankAppear = false;
						isAllowSuperTankMove = true;
					}
				}

				if (isAllowRenderSuperTank) {
					superTankBoss->render(renderer, camera);
					if (isAllowSuperTankMove) {
						superTankBoss->handleSwitchLevel(tank.getTankCircle());
						superTankBoss->move();
						superTankBoss->createBullet(renderer);
						tank.setDamgeReceived(superTankBoss->handleBullet(map, renderer, camera, tank.getTankCircle()));
						superTankBoss->renderBullet(renderer, camera);
					}
				}

				if (isAllowCreateTankBossList) {
					bossList.createListBoss(map, tank.getTankCircle(), level * 5, level < 5 ? level : 4, renderer, level * 50, level * 2);
					isAllowCreateTankBossList = false;
				}

				if (level >= 1 && SDL_GetTicks() - itemList.getSaveTimeCreate() >= TIME_RECREATE_ITEM) {
					itemList.createList(renderer, map);
				}

				if (isAllowTankMainMove) {
					if (isAllowRenderSuperTank) {
						tank.move(map, bossList, superTankBoss->getTankCircle(), superTankBoss->getCircleBallFire());
						bossList.handleList(map, tank.getTankCircle(), renderer, camera, itemList, superTankBoss->getCircleBallFire());
					}
					else {
						tank.move(map, bossList, { 0, 0, 0 }, { 0, 0, 0 });
						bossList.handleList(map, tank.getTankCircle(), renderer, camera, itemList, { 0, 0, 0 });
						if (!isSuperTankAppear && bossList.getQualityBoss() == 0) {
							isLevelUp = true;
							std::cout << level << std::endl;
						}
					}
					tank.handleDamgeReceived(renderer, smallFont);
					
				}
				
				tank.setCamera(camera);
				
				if (level >= 1) {
					itemList.handleList();
					tank.handleEatItem(itemList.getItemList(), renderer, smallFont);
				}
				tank.createBullet(renderer);
				bool _isSlowedTankMain = false;

				if (isAllowRenderSuperTank) { // có super tank
					tank.setDamgeReceived(bossList.handleBulletOfTankList(map, renderer, camera, tank.getTankCircle(), _isSlowedTankMain, superTankBoss->getCircleBallFire()));
					tank.handleBullet(map, renderer, camera, bossList, superTankBoss);
					superTankBoss->handleDamgeReceived(renderer, font);
				}
				else { // không có super tank
					tank.setDamgeReceived(bossList.handleBulletOfTankList(map, renderer, camera, tank.getTankCircle(), _isSlowedTankMain, { 0, 0, 0 }));
					tank.handleBullet(map, renderer, camera, bossList, NULL);
				}

				if (_isSlowedTankMain) {
					tank.setIsSlowed(true);
					tank.setSaveTimeIsSlowed(SDL_GetTicks());
				}

				

				/*if (!isSuperTankAppear && bossList.getQualityBoss() == 0) {
					isLevelUp = true;
				}*/
				itemList.renderItemlist(renderer, camera);
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