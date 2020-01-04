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
		bool isLevelUp = false;
		bool isLoadedSuperTank = false;
		bool isAllowRenderEffectSuperTank = false;
		bool isMoveTankMain = true;
		bool isAllowHandleAppearSuperTank = false;
		if (load()) {
			SDL_Rect camera = { 0, 0, cameraWidth, cameraHeight }; // khai báo camera
			SDL_ShowCursor(SDL_DISABLE); // ẩn con trỏ chuột
			bossList.createListBoss(map, tank.getTankCircle(), level * 5, level < 5 ? level : 4, renderer, level * 50, level * 2);
			while (!out) {
				while (SDL_PollEvent(&event) != 0) { // bắt các sự kiện
					if (event.type == SDL_QUIT) {
						out = true;
					}
					tank.handleEvents(&event, camera);
				}

				if (isLevelUp) { // tăng độ level game
					level++;
					bossList.createListBoss(map, tank.getTankCircle(), level * 5, level < 5 ? level : 4, renderer, level * 50, level * 2);
					isLevelUp = false;
					std::cout << "level up" << std::endl;
				}

				if (level >= 1 && SDL_GetTicks() - itemList.getSaveTimeCreate() >= TIME_RECREATE_ITEM) {
					itemList.createList(renderer, map);
				}

				SDL_RenderClear(renderer); // clear màn hình render
				SDL_SetRenderDrawColor(renderer, 100, 50, 0, 0);
				map.render(renderer, camera);
				if (level == TOTAL_LEVEL_GAME && !isLoadedSuperTank) {
					SuperTankBoss* _superTank = new SuperTankBoss;

					_superTank->loadImg("./image/super_tank_boss_2.png", renderer);
					_superTank->loadEffectAppear(renderer);
					_superTank->handleEffectAppear(map.getMapWallDigital());
				//	_superTank->load_ball_fire(renderer, "image\\ball_fire_3.png");
				//	_superTank->load_ds_bullet_reuse(renderer);
				//	_superTank->load_img_shield(renderer, "image\\shield.png");
				//	_superTank->set_is_destroy(false);
					isLoadedSuperTank = true;
					isAllowRenderEffectSuperTank = true;
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
				if (isAllowHandleAppearSuperTank && !isAllowRenderEffectSuperTank)
				{
					map.reload();
					map.render(renderer, camera);
					isMoveTankMain = false;
					//main_char.handle_move_automatic(width_background / 2 - main_char.get_width() / 2, height_background - 90 - main_char.get_height());
					//main_char.set_box_x_y(width_background / 2 - main_char.get_width() / 2, height_background - 82 - main_char.get_height());
					//Tank_boss->set_box_x_y(width_background / 2 - Tank_boss->get_width() / 2, height_background - 2 * height_camera);
					isAllowHandleAppearSuperTank = false;
					//is_appear_boss = true;
				}
				tank.move(map, bossList);
				tank.setCamera(camera);
				bossList.handleList(map, tank.getTankCircle(), renderer, camera, itemList);
				if (level >= 1) {
					itemList.handleList();
					tank.handleEatItem(itemList.getItemList(), renderer, smallFont);
				}
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