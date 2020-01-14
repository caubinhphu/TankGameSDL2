#include "general.h"
#include "TankMain.h"
#include "MapGame.h";
#include "TankBoss.h"
#include "Item.h"
#include "SuperTankBoss.h"

#define TOTAL_LEVEL_GAME 5

TankMain tank(100, 100);

BasicObj menuBacground;
BasicObj homeBacground;
BasicObj shopBackground;
BasicObj aboutBackground;
BasicObj winBackground;
BasicObj pauseBackground;
BasicObj gameOverBackground;
BasicObj warning;

BasicObj textMenu[4];
BasicObj textMainHome[4];
BasicObj textHome[4];
BasicObj textGunHome[4];
BasicObj textAbout;
BasicObj textPause[2];
BasicObj textWin;
BasicObj textGameOver;

BasicObj frameHome[11];
BasicObj frameGunHome[3];

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
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
		return false;
	return true;
}

void loadFontSound() {
	// load font
	font = TTF_OpenFont("./font/turok.ttf", 30);
	smallFont = TTF_OpenFont("./font/turok.ttf", 25);
	bigFont = TTF_OpenFont("./font/turok.ttf", 50);

	// load sound
	musicBackgroundGame = Mix_LoadMUS("./sound/background2.mp3");
	musicMenuMain = Mix_LoadMUS("./sound/menu_main.mp3");
	musicMenuHome = Mix_LoadMUS("./sound/menu_home.mp3");
	musicMenuShop = Mix_LoadMUS("./sound/menu_shop.mp3");

	musicChunk[SOUND_SHOOT_FIRE] = Mix_LoadWAV("./sound/shoot_fire.wav");
	musicChunk[SOUND_SHOOT_NORMAL] = Mix_LoadWAV("./sound/shoot_normal.wav");
	musicChunk[SOUND_COLLISION_WALL] = Mix_LoadWAV("./sound/collision_wall.wav");
	musicChunk[SOUND_COLLISION_ENEMIES] = Mix_LoadWAV("./sound/collision_2.wav");
	musicChunk[SOUND_NULL_BULLET] = Mix_LoadWAV("./sound/null_ammo.wav");
	musicChunk[SOUND_EXPLOSION] = Mix_LoadWAV("./sound/explosion.wav");
	musicChunk[SOUND_MOUSE_INSIDE] = Mix_LoadWAV("./sound/effect_mouse_inside_2.wav");
	musicChunk[SOUND_SHOOT_ROCKET] = Mix_LoadWAV("./sound/shoot_rocket.wav");

	for (int i = 0; i < 8; i++)
		musicChunk[i]->volume = 400;
}

void loadMenuMain() {

	//load font + sound
	loadFontSound();

	// load menu chính
	menuBacground.loadImg("./image/background_menu.png", renderer);
	SDL_Color _color = { 255, 255, 255 };
	textMenu[0].loadText(bigFont, "Continue", _color, renderer);
	textMenu[1].loadText(font, "New Game", _color, renderer);
	textMenu[2].loadText(font, "About", _color, renderer);
	textMenu[3].loadText(font, "Exit", _color, renderer);
	int _t = 250;
	for (int i = 0; i < 4; i++) {
		textMenu[i].setXY(cameraWidth / 2 - textMenu[i].getW() / 2, _t);
		_t += textMenu[i].getH() + 15;
	}
}

void loadAboutMenu() {
	// load menu About
	SDL_Color _color = { 255, 255, 255 };
	aboutBackground.loadImg("./image/background_about.png", renderer);
	textAbout.loadText(bigFont, "BACK", _color, renderer);
	textAbout.setXY(525, 395);
}

void loadHomeMenu() {
	// load menu home
	SDL_Color _color = { 255, 255, 255 };

	homeBacground.loadImg("./image/background_home.png", renderer);

	textMainHome[0].loadText(bigFont, "Play", _color, renderer);
	textMainHome[1].loadText(bigFont, "Shop", _color, renderer);
	textMainHome[2].loadText(bigFont, "Back", _color, renderer);
	textMainHome[3].loadText(bigFont, "Save", _color, renderer);

	textMainHome[0].setXY(490, cameraHeight - textMainHome[0].getH());
	textMainHome[1].setXY(220, cameraHeight - textMainHome[1].getH());
	textMainHome[2].setXY(20, cameraHeight - textMainHome[2].getH());
	textMainHome[3].setXY(480, 310);

	frameHome[0].loadImg("./image/frame_money.png", renderer);//tien
	for (int i = 1; i <= 3; i++)
		frameHome[i].loadImg("./image/frame_weapon.png", renderer);//vu khi
	frameHome[4].loadImg("./image/frame_shield.png", renderer);//giap
	frameHome[5].loadImg("./image/frame_wheel.png", renderer);//banh xe
	frameHome[6].loadImg("./image/frame_power.png", renderer);//nang luong
	frameHome[7].loadImg("./image/frame_health.png", renderer);//mau
	frameHome[8].loadImg("./image/frame.png", renderer);//khung chi so
	frameHome[9].loadImg("./image/frame_upgrade.png", renderer);//nang cap
	frameHome[10].loadImg("./image/double_frame.png", renderer);//khung kep

	frameHome[0].setXY(420, 55);
	frameHome[1].setXY(420, 135);
	frameHome[2].setXY(420, 225);
	frameHome[3].setXY(420, 300);
	frameHome[4].setXY(55, 115);
	frameHome[5].setXY(55, 270);
	frameHome[6].setXY(305, 115);
	frameHome[7].setXY(305, 270);

	warning.loadImg("./image/warning.png", renderer);
	warning.setXY(600, 60);

	frameGunHome[0].loadImg("./image/TD_gun.png", renderer);
	frameGunHome[1].loadImg("./image/fire_gun.png", renderer);
	frameGunHome[2].loadImg("./image/rocket_gun.png", renderer);
}

void loadPauseMenu() {
	// load menu Pause
	SDL_Color _color = { 255, 255, 255 };
	pauseBackground.loadImg("./image/background_pause.png", renderer);
	textPause[0].loadText(bigFont, "RESUME", _color, renderer);
	textPause[0].setXY(242, 237);
	textPause[1].loadText(bigFont, "BACK TO HOME", _color, renderer);
	textPause[1].setXY(202, 320);
}

void loadGame() {
	SDL_Color _colorRed = { 255, 0, 0 };
	SDL_Color _colorPurple = { 163, 73, 164 };

	if (tank.getSlotGun1Type() == Bullet::nomalBullet) {
		textGunHome[0].loadText(smallFont, "T-D Gun", _colorRed, renderer);
		textGunHome[2].loadText(smallFont, "OO", _colorPurple, renderer);
	}
	else if (tank.getSlotGun1Type() == Bullet::fireBullet) {
		textGunHome[0].loadText(smallFont, "GJKD_Fire", _colorRed, renderer);
		std::stringstream _t("");
		_t << tank.getTotalFireBullet();
		textGunHome[2].loadText(smallFont, _t.str(), _colorPurple, renderer);
	}
	else if (tank.getSlotGun1Type() == Bullet::rocketBullet) {
		textGunHome[0].loadText(smallFont, "GS3_Rocket", _colorRed, renderer);
		std::stringstream _t("");
		_t << tank.getTotalRocketBullet();
		textGunHome[2].loadText(smallFont, _t.str(), _colorPurple, renderer);
	}

	if (tank.getSlotGun2Type() == Bullet::nomalBullet) {
		textGunHome[1].loadText(smallFont, "T-D Gun", _colorRed, renderer);
		textGunHome[3].loadText(smallFont, "OO", _colorPurple, renderer);
	}
	else if (tank.getSlotGun2Type() == Bullet::fireBullet) {
		textGunHome[1].loadText(smallFont, "GJKD_Fire", _colorRed, renderer);
		std::stringstream _t("");
		_t << tank.getTotalFireBullet();
		textGunHome[3].loadText(smallFont, _t.str(), _colorPurple, renderer);
	}
	else if (tank.getSlotGun2Type() == Bullet::rocketBullet) {
		textGunHome[1].loadText(smallFont, "GS3_Rocket", _colorRed, renderer);
		std::stringstream _t("");
		_t << tank.getTotalRocketBullet();
		textGunHome[3].loadText(smallFont, _t.str(), _colorPurple, renderer);
	}
}

int handleMenuPause() {
	bool out = false;
	std::vector<bool> flagChuck(2, false);
	SDL_ShowCursor(SDL_ENABLE);
	while (!out) {
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT)
				out = true;
			else if (e.type == SDL_KEYUP) {
				if (e.key.keysym.sym == SDLK_ESCAPE) {
					SDL_ShowCursor(SDL_DISABLE);
					return 0;
				}
			}
			else if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
				int _x = 0, _y = 0;
				SDL_GetMouseState(&_x, &_y); // lấy tọa độ chuột
				for (int i = 0; i < 2; i++) {
					if (check::checkInsideRect(_x, _y, textPause[i].getBox())) {
						if (!flagChuck[i]) {
							Mix_PlayChannel(-1, musicChunk[SOUND_MOUSE_INSIDE], 0);
							flagChuck[i] = true;
						}
						textPause[i].setColor(0, 0, 128);
						if (e.type == SDL_MOUSEBUTTONDOWN) {
							if (i == 0) { // chơi tiếp
								SDL_ShowCursor(SDL_DISABLE);
								return 0;
							}
							else if (i == 1) { // về home
								Mix_HaltMusic();
								return 1;
							}
						}
					}
					else {
						textPause[i].setColor(255, 255, 255);
						flagChuck[i] = false;
					}
				}
			}
		}
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 100, 100, 0);
		pauseBackground.render(renderer, 0, 0, NULL, 0);
		for (int i = 0; i < 2; i++)
			textPause[i].render(renderer, textPause[i].getX(), textPause[i].getY(), NULL, 0);

		SDL_RenderPresent(renderer);
	}
}

void handleMenuAbout() {
	bool out = false;
	bool flagChuck = false;
	while (!out) {
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			if (Mix_PlayingMusic() == 0)
				Mix_PlayMusic(musicMenuMain, -1);
			if (e.type == SDL_QUIT)
				out = true;
			if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
				int _x = 0, _y = 0;
				SDL_GetMouseState(&_x, &_y); // lấy tọa độ chuột
				if (check::checkInsideRect(_x, _y, textAbout.getBox())) {
					if (!flagChuck) {
						Mix_PlayChannel(-1, musicChunk[SOUND_MOUSE_INSIDE], 0);
						flagChuck = true;
					}
					textAbout.setColor(0, 0, 128);
					if (e.type == SDL_MOUSEBUTTONDOWN) {
						return;
					}
				}
				else {
					textAbout.setColor(255, 255, 255);
					flagChuck = false;
				}
			}
		}
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 100, 100, 0);
		aboutBackground.render(renderer, 0, 0, NULL, 0);
		textAbout.render(renderer, textAbout.getX(), textAbout.getY(), NULL, 0);

		SDL_RenderPresent(renderer);
	}
}

int handleMenuGame() {
	bool out = false;
	std::vector<bool> flagChuck(4, false);
	while (!out) {
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			if (Mix_PlayingMusic() == 0)
				Mix_PlayMusic(musicMenuMain, -1);
			if (e.type == SDL_QUIT)
				out = true;
			if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
				int _x = 0, _y = 0;
				SDL_GetMouseState(&_x, &_y); // lấy tọa độ chuột
				for (int i = 0; i < 4; i++) {
					if (check::checkInsideRect(_x, _y, textMenu[i].getBox())) {
						if (!flagChuck[i]) {
							Mix_PlayChannel(-1, musicChunk[SOUND_MOUSE_INSIDE], 0);
							flagChuck[i] = true;
						}
						textMenu[i].setColor(255, 0, 0);
						if (e.type == SDL_MOUSEBUTTONDOWN) {
							if (i == 0) { // Continue
								Mix_HaltMusic();
								return 0;
							}
							else if (i == 1) { // new game
								Mix_HaltMusic();
								return 1;
							}
							else if (i == 2) { // about
								return 2;
							}
							else if (i == 3) { // exit
								return 3;
							}
						}
					}
					else {
						textMenu[i].setColor(255, 255, 255);
						flagChuck[i] = false;
					}
				}
			}
		}
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 100, 100, 0);
		menuBacground.render(renderer, 0, 0, NULL, 0);
		for (int i = 0; i < 4; i++)
		{
			textMenu[i].render(renderer, textMenu[i].getX(), textMenu[i].getY(), NULL, 0);
		}
		SDL_RenderPresent(renderer);
	}
	return 3;
}

void game();

void saveGame() {
	;
}

void handleMenuHome() {
	bool out = false;
	std::vector<bool> flagChuck(11, false);
	std::vector<bool> flagChuckMainText(4, false);
	bool isRenderFrame89 = false;
	bool isRenderFrame10 = false;
	bool isRenderFrameGun = false;
	bool isMouseInsideFrameGun = false;
	int indexFrameHome = -1;
	SDL_Event e;
	SDL_Color _color = { 255, 0, 0 };
	SDL_Color _colorPurple = { 163, 73, 164 };
	bool isRenderWarning = false;
	if (Mix_PlayingMusic() == 0)
		Mix_PlayMusic(musicMenuHome, -1);

	std::stringstream _t("");
	_t << tank.getMoney();
	textHome[0].loadText(font, _t.str(), _color, renderer);

	while (!out) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				out = true;
				return;
			}
			if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
				int _x = 0, _y = 0;
				SDL_GetMouseState(&_x, &_y);
				if (isRenderFrame89) {
					if (check::checkInsideRect(_x, _y, frameHome[9].getBox())) {
						frameHome[10].setXY(frameHome[9].getX() + frameHome[9].getW() + 4, frameHome[9].getY());
						isRenderFrame10 = true;
						if (e.type == SDL_MOUSEBUTTONDOWN) {
							if (indexFrameHome == 4) {
								if (tank.getMoney() >= tank.getMoneyNeedUpgradeArmor()) {
									tank.setChangeMoney(-tank.getMoneyNeedUpgradeArmor(), smallFont, renderer);
									tank.setPlusArmor(PLUS_UPGRADE_ARMOR);
									tank.setMoneyNeedUpgradeArmor();

									std::stringstream _t("");
									_t << tank.getArmor();
									textHome[1].loadText(smallFont, _t.str(), _color, renderer);

									_t.str("");
									_t << "-" << tank.getMoneyNeedUpgradeArmor();
									textHome[3].loadText(smallFont, _t.str(), _color, renderer);
								}
								else isRenderWarning = true;
							}
							else if (indexFrameHome == 5) {
								if (tank.getMoney() >= tank.getMoneyNeedUpgradeSpeed()) {
									tank.setChangeMoney(-tank.getMoneyNeedUpgradeSpeed(), smallFont, renderer);
									tank.setPlusSpeed(PLUS_UPGRADE_SPEED);
									tank.setMoneyNeedUpgradeSpeed();

									std::stringstream _t("");
									_t << tank.getSpeed();
									textHome[1].loadText(smallFont, _t.str(), _color, renderer);

									_t.str("");
									_t << "-" << tank.getMoneyNeedUpgradeSpeed();
									textHome[3].loadText(smallFont, _t.str(), _color, renderer);
								}
								else isRenderWarning = true;
							}
							else if (indexFrameHome == 6) {
								if (tank.getMoney() >= tank.getMoneyNeedUpgradePower()) {
									tank.setChangeMoney(-tank.getMoneyNeedUpgradePower(), smallFont, renderer);
									tank.setPlusPower(PLUS_UPGRADE_POWER);
									tank.setMoneyNeedUpgradePower();

									std::stringstream _t("");
									_t << tank.getPower();
									textHome[1].loadText(smallFont, _t.str(), _color, renderer);

									_t.str("");
									_t << "-" << tank.getMoneyNeedUpgradePower();
									textHome[3].loadText(smallFont, _t.str(), _color, renderer);
								}
								else isRenderWarning = true;
							}
							else if (indexFrameHome == 7) {
								if (tank.getMoney() >= tank.getMoneyNeedUpgradeTotalHealth()) {
									tank.setChangeMoney(-tank.getMoneyNeedUpgradeTotalHealth(), font, renderer);
									tank.setPlusTotalHealth(PLUS_UPGRADE_HEALTH);
									tank.setMoneyNeedUpgradeTotalHealth();

									std::stringstream _t("");
									_t << tank.getTotalHealth();
									textHome[1].loadText(smallFont, _t.str(), _color, renderer);

									_t.str("");
									_t << "-" << tank.getMoneyNeedUpgradeTotalHealth();
									textHome[3].loadText(smallFont, _t.str(), _color, renderer);
								}
								else isRenderWarning = true;
							}
						}
					}
					else isRenderFrame10 = false;
				}

				if (isRenderFrameGun) {
					std::stringstream _bullet("");

					for (int l = 0; l < 3; l++)
					{
						if (check::checkInsideRect(_x, _y, frameGunHome[l].getBox())) {
							if (e.type == SDL_MOUSEBUTTONDOWN) {
								if (l == 0) {
									if (indexFrameHome == 1) {
										textGunHome[0].loadText(smallFont, "T-D Gun", _color, renderer);
										tank.setSlotGun1Type(Bullet::nomalBullet);
										textGunHome[2].loadText(smallFont, "OO", _colorPurple, renderer);
									}
									else if (indexFrameHome == 2) {
										textGunHome[1].loadText(smallFont, "T-D Gun", _color, renderer);
										tank.setSlotGun2Type(Bullet::nomalBullet);
										textGunHome[3].loadText(smallFont, "OO", _colorPurple, renderer);
									}
								}
								else if (l == 1 && tank.getIsHaveFireGun()) {
									if (indexFrameHome == 1) {
										textHome[0].loadText(smallFont, "GJKD_Fire", _color, renderer);
										tank.setSlotGun1Type(Bullet::fireBullet);
										_bullet.str("");
										_bullet << tank.getTotalFireBullet();
										textGunHome[2].loadText(smallFont, _bullet.str(), _colorPurple, renderer);
									}
									else if (indexFrameHome == 2) {
										textGunHome[1].loadText(smallFont, "GJKD_Fire", _color, renderer);
										tank.setSlotGun2Type(Bullet::fireBullet);
										_bullet.str("");
										_bullet << tank.getTotalFireBullet();
										textGunHome[3].loadText(smallFont, _bullet.str(), _colorPurple, renderer);
									}
								}
								else if (l == 2 && tank.getIsHaveRocketGun()) {
									if (indexFrameHome == 1) {
										textGunHome[0].loadText(smallFont, "GS3_Rocket", _color, renderer);
										tank.setSlotGun1Type(Bullet::rocketBullet);
										_bullet.str("");
										_bullet << tank.getTotalRocketBullet();
										textGunHome[2].loadText(smallFont, _bullet.str(), _colorPurple, renderer);
									}
									else if (indexFrameHome == 2) {
										textGunHome[1].loadText(smallFont, "GS3_Rocket", _color, renderer);
										tank.setSlotGun2Type(Bullet::rocketBullet);
										_bullet.str("");
										_bullet << tank.getTotalRocketBullet();
										textGunHome[3].loadText(smallFont, _bullet.str(), _colorPurple, renderer);
									}
								}
							}
							break;
						}
						else if (l >= 2) {
							if (isMouseInsideFrameGun == false) isRenderFrameGun = false;
						}
					}
				}

				for (int i = 0; i < 8; i++) {
					if (check::checkInsideRect(_x, _y, frameHome[i].getBox())) {
						if (flagChuck[i] == false) {
							Mix_PlayChannel(-1, musicChunk[SOUND_MOUSE_INSIDE], 0);
							flagChuck[i] = true;
							for (int j = 0; j < 8; j++) {
								if (j != i) flagChuck[j] = false;
							}
						}
						if (i == 4 || i == 5 || i == 6 || i == 7) {
							isMouseInsideFrameGun = false;
							frameHome[8].setXY(frameHome[i].getX() - 15, frameHome[i].getY() - frameHome[8].getH());
							frameHome[9].setXY(frameHome[8].getX() + frameHome[8].getW() + 25, frameHome[8].getY());
							isRenderFrame89 = true;

							if (i == 4) {
								std::stringstream _t("");
								_t << tank.getArmor();
								textHome[1].loadText(smallFont, _t.str(), _color, renderer);

								_t.str("");
								_t << "+" << PLUS_UPGRADE_ARMOR;
								textHome[2].loadText(smallFont, _t.str(), _color, renderer);

								_t.str("");
								_t << "-" << tank.getMoneyNeedUpgradeArmor();
								textHome[3].loadText(smallFont, _t.str(), _color, renderer);

								indexFrameHome = 4;
							}
							else if (i == 5) {
								std::stringstream _t("");
								_t << tank.getSpeed();
								textHome[1].loadText(smallFont, _t.str(), _color, renderer);

								_t.str("");
								_t << "+" << PLUS_UPGRADE_SPEED;
								textHome[2].loadText(smallFont, _t.str(), _color, renderer);

								_t.str("");
								_t << "-" << tank.getMoneyNeedUpgradeSpeed();
								textHome[3].loadText(smallFont, _t.str(), _color, renderer);

								indexFrameHome = 5;
							}
							else if (i == 6) {
								std::stringstream _t("");
								_t << tank.getPower();
								textHome[1].loadText(smallFont, _t.str(), _color, renderer);

								_t.str("");
								_t << "+" << PLUS_UPGRADE_POWER;
								textHome[2].loadText(smallFont, _t.str(), _color, renderer);

								_t.str("");
								_t << "-" << tank.getMoneyNeedUpgradePower();
								textHome[3].loadText(smallFont, _t.str(), _color, renderer);

								indexFrameHome = 6;
							}
							else if (i == 7) {
								std::stringstream _t("");
								_t << tank.getTotalHealth();
								textHome[1].loadText(smallFont, _t.str(), _color, renderer);

								_t.str("");
								_t << "+" << PLUS_UPGRADE_HEALTH;
								textHome[2].loadText(smallFont, _t.str(), _color, renderer);

								_t.str("");
								_t << "-" << tank.getMoneyNeedUpgradeTotalHealth();
								textHome[3].loadText(smallFont, _t.str(), _color, renderer);

								indexFrameHome = 7;
							}
						}
						else if (i == 1 || i == 2) {
							isMouseInsideFrameGun = true;
							frameGunHome[0].setXY(frameHome[i].getX(), frameHome[i].getY() - frameGunHome[0].getH());
							frameGunHome[1].setXY(frameGunHome[0].getX() + frameGunHome[0].getW() + 5, frameHome[i].getY() - frameGunHome[0].getH());
							frameGunHome[2].setXY(frameGunHome[1].getX() + frameGunHome[1].getW() + 5, frameHome[i].getY() - frameGunHome[0].getH());
							isRenderFrameGun = true;
							if (i == 1)
								indexFrameHome = 1;
							else if (i == 2)
								indexFrameHome = 2;

						}
						break;
					}
					else if (i >= 7)
					{
						if (isRenderFrame10 == false) isRenderFrame89 = false;
						flagChuck.assign(11, false);
						isMouseInsideFrameGun = false;
					}
				}

				for (int k = 0; k < 4; k++) {
					if (check::checkInsideRect(_x, _y, textMainHome[k].getBox())) {
						textMainHome[k].setColor(0, 0, 128);
						if (flagChuckMainText[k] == false) {
							Mix_PlayChannel(-1, musicChunk[SOUND_MOUSE_INSIDE], 0);
							flagChuckMainText[k] = true;
						}
						if (e.type == SDL_MOUSEBUTTONDOWN) {
							if (k == 0) { // chơi game
								Mix_HaltMusic();
								game();

								std::stringstream _bullet("");
								if (tank.getSlotGun1Type() == Bullet::fireBullet) {
									_bullet.str("");
									_bullet << tank.getTotalFireBullet();
									textGunHome[2].loadText(smallFont, _bullet.str(), _colorPurple, renderer);
								}
								else if (tank.getSlotGun1Type() == Bullet::rocketBullet) {
									_bullet.str("");
									_bullet << tank.getTotalRocketBullet();
									textGunHome[2].loadText(smallFont, _bullet.str(), _colorPurple, renderer);
								}

								if (tank.getSlotGun2Type() == Bullet::fireBullet) {
									_bullet.str("");
									_bullet << tank.getTotalFireBullet();
									textGunHome[3].loadText(smallFont, _bullet.str(), _colorPurple, renderer);
								}
								else if (tank.getSlotGun2Type() == Bullet::rocketBullet) {
									_bullet.str("");
									_bullet << tank.getTotalRocketBullet();
									textGunHome[3].loadText(smallFont, _bullet.str(), _colorPurple, renderer);
								}

								_t = std::stringstream("");
								_t << tank.getMoney();
								textHome[0].loadText(font, _t.str(), _color, renderer);
							}
							else if (k == 1) { // vào shop
								// Mix_HaltMusic();

								// if (menu_shop() == 9) return 9;

								/*std::stringstream _bullet("");
								if (main_char.get_type_gun_slot_1() == AMMO::fire)
								{
									_bullet.str("");
									_bullet << main_char.get_total_ammo();
									text_gun_home[2].load_text(small_font, _bullet.str(), _color_purple, renderer);
								}
								else if (main_char.get_type_gun_slot_1() == AMMO::rocket)
								{
									_bullet.str("");
									_bullet << main_char.get_total_bullet_rocket();
									text_gun_home[2].load_text(small_font, _bullet.str(), _color_purple, renderer);
								}

								if (main_char.get_type_gun_slot_2() == AMMO::fire)
								{
									_bullet.str("");
									_bullet << main_char.get_total_ammo();
									text_gun_home[3].load_text(small_font, _bullet.str(), _color_purple, renderer);
								}
								else if (main_char.get_type_gun_slot_2() == AMMO::rocket)
								{
									_bullet.str("");
									_bullet << main_char.get_total_bullet_rocket();
									text_gun_home[3].load_text(small_font, _bullet.str(), _color_purple, renderer);
								}*/
								return;
							}
							else if (k == 2) // back
							{
								Mix_HaltMusic();
								return;
							}
							else if (k == 3) { // save game
								saveGame();
							}
						}
						break;
					}
					else {
						textMainHome[k].setColor(255, 255, 255);
						if (k >= 3) flagChuckMainText.assign(4, false);
					}
				}
			}
		}

		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 100, 100, 0);

		homeBacground.render(renderer, 0, 0, NULL, 0);

		frameHome[0].render(renderer, 420, 45, NULL, 0);
		frameHome[1].render(renderer, 420, 135, NULL, 0);
		frameHome[2].render(renderer, 420, 225, NULL, 0);
//		frameHome[3].render(renderer, 420, 300, NULL, 0);
		frameHome[4].render(renderer, 55, 115, NULL, 0);
		frameHome[5].render(renderer, 55, 270, NULL, 0);
		frameHome[6].render(renderer, 305, 115, NULL, 0);
		frameHome[7].render(renderer, 305, 270, NULL, 0);

		textHome[0].render(renderer, 480, 55, NULL, 0);

		if (isRenderFrame89) {
			frameHome[8].render(renderer, frameHome[8].getX(), frameHome[8].getY(), NULL, 0);
			frameHome[9].render(renderer, frameHome[9].getX(), frameHome[9].getY(), NULL, 0);
			textHome[1].render(renderer, frameHome[8].getX() + frameHome[8].getW() / 2 - textHome[1].getW() / 2, frameHome[8].getY() + 6, NULL, 0);
		}

		if (isRenderFrame10) {
			frameHome[10].render(renderer, frameHome[10].getX(), frameHome[10].getY(), NULL, 0);
			textHome[2].render(renderer, frameHome[10].getX(), frameHome[10].getY(), NULL, 0);
			textHome[3].render(renderer, frameHome[10].getX(), frameHome[10].getY() + textHome[2].getH() - 8, NULL, 0);
		}

		if (isRenderWarning)
			warning.render(renderer, 600, 50, NULL, 0);
		if (isRenderFrameGun) {
			frameGunHome[0].render(renderer, frameGunHome[0].getX(), frameGunHome[0].getY(), NULL, 0);
			if (tank.getIsHaveFireGun())
				frameGunHome[1].render(renderer, frameGunHome[1].getX(), frameGunHome[1].getY(), NULL, 0);
			if (tank.getIsHaveRocketGun())
				frameGunHome[2].render(renderer, frameGunHome[2].getX(), frameGunHome[2].getY(), NULL, 0);
		}

		textGunHome[0].render(renderer, 460, 145, NULL, 0);
		textGunHome[1].render(renderer, 460, 235, NULL, 0);
		textGunHome[2].render(renderer, cameraWidth - textGunHome[2].getW() - 5, 160, NULL, 0);
		textGunHome[3].render(renderer, cameraWidth - textGunHome[3].getW() - 5, 250, NULL, 0);

		for (int i = 0; i < 4; i++)
			textMainHome[i].render(renderer, textMainHome[i].getX(), textMainHome[i].getY(), NULL, 0);
		SDL_RenderPresent(renderer);
		}
	return;
}

bool load() {
	if (!tank.loadImg(renderer)) {
		return false;
	}
	if (!winBackground.loadImg("./image/youwin.png", renderer)) {
		return false;
	}
	textWin.loadText(bigFont, "BACK", { 255, 255, 255 }, renderer);
	textWin.setXY(525, 395);
	if (!gameOverBackground.loadImg("./image/background_destroy.png", renderer)) {
		return false;
	}
	textGameOver.loadText(bigFont, "BACK", { 255, 255, 255 }, renderer);
	textGameOver.setXY(525, 395);

	return true;
}

void handleGameOverMenu() {
	bool out = false;
	bool flagChuck = false;
	SDL_ShowCursor(SDL_ENABLE);
	while (!out) {
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT)
				out = true;
			if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
				int _x = 0, _y = 0;
				SDL_GetMouseState(&_x, &_y); // lấy tọa độ chuột
				if (check::checkInsideRect(_x, _y, textGameOver.getBox())) {
					if (!flagChuck) {
						Mix_PlayChannel(-1, musicChunk[SOUND_MOUSE_INSIDE], 0);
						flagChuck = true;
					}
					textGameOver.setColor(255, 0, 0);
					if (e.type == SDL_MOUSEBUTTONDOWN) {
						return;
					}
				}
				else {
					textGameOver.setColor(255, 255, 255);
					flagChuck = false;
				}
			}
		}
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 100, 100, 0);
		gameOverBackground.render(renderer, 0, 0, NULL, 0);
		textGameOver.render(renderer, textGameOver.getX(), textGameOver.getY(), NULL, 0);

		SDL_RenderPresent(renderer);
	}
}

void handleWinMenu() {
	bool out = false;
	bool flagChuck = false;
	SDL_ShowCursor(SDL_ENABLE);
	while (!out) {
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT)
				out = true;
			if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
				int _x = 0, _y = 0;
				SDL_GetMouseState(&_x, &_y); // lấy tọa độ chuột
				if (check::checkInsideRect(_x, _y, textWin.getBox())) {
					if (!flagChuck) {
						Mix_PlayChannel(-1, musicChunk[SOUND_MOUSE_INSIDE], 0);
						flagChuck = true;
					}
					textWin.setColor(255, 0, 0);
					if (e.type == SDL_MOUSEBUTTONDOWN) {
						return;
					}
				}
				else {
					textWin.setColor(255, 255, 255);
					flagChuck = false;
				}
			}
		}
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 100, 100, 0);
		winBackground.render(renderer, 0, 0, NULL, 0);
		textWin.render(renderer, textWin.getX(), textWin.getY(), NULL, 0);

		SDL_RenderPresent(renderer);
	}
}

void loadDataTank(bool _isNewGame) {
	std::string pathDataFile;
	if (_isNewGame) {
		pathDataFile = "dulieu.txt";
	}
	else {
		pathDataFile = "dulieu2.txt";
	}
	std::ifstream _dataTank(pathDataFile, std::ios::in);
	std::string _data;
	getline(_dataTank, _data); tank.setTotalHealth(atoi(_data.c_str()));
	getline(_dataTank, _data); tank.setArmor(atoi(_data.c_str()));
	getline(_dataTank, _data); tank.setPower(atoi(_data.c_str()));
	getline(_dataTank, _data); tank.setSpeed(atoi(_data.c_str()));
	getline(_dataTank, _data); tank.setMoney(atoi(_data.c_str()), smallFont, renderer);
	getline(_dataTank, _data); tank.setTotalMoneyNeedUpgradeArmor(atoi(_data.c_str()));
	getline(_dataTank, _data); tank.setTotalMoneyNeedUpgradeTotalHealth(atoi(_data.c_str()));
	getline(_dataTank, _data); tank.setTotalMoneyNeedUpgradePower(atoi(_data.c_str()));
	getline(_dataTank, _data); tank.setTotalMoneyNeedUpgradeSpeed(atoi(_data.c_str()));
	getline(_dataTank, _data); tank.setTotalFireBullet(atoi(_data.c_str()));
	getline(_dataTank, _data); tank.setPlusTotalRocketBullet(atoi(_data.c_str()));
	getline(_dataTank, _data); tank.setIsHaveFireGun(atoi(_data.c_str()));
	getline(_dataTank, _data); tank.setIsHaveRocketGun(atoi(_data.c_str()));
	getline(_dataTank, _data);
	{
		if (_data == "T-D Gun") tank.setSlotGun1Type(Bullet::nomalBullet);
		else if (_data == "GJKD_Fire") tank.setSlotGun1Type(Bullet::fireBullet);
		else if (_data == "GS3_Rocket") tank.setSlotGun1Type(Bullet::rocketBullet);
	}
	getline(_dataTank, _data);
	{
		if (_data == "T-D Gun") tank.setSlotGun2Type(Bullet::nomalBullet);
		else if (_data == "GJKD_Fire") tank.setSlotGun2Type(Bullet::fireBullet);
		else if (_data == "GS3_Rocket") tank.setSlotGun2Type(Bullet::rocketBullet);
	}
	tank.loadGun12(renderer);
}

void close() {
	tank.free();
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
}

void game() {
	srand(time(NULL));
	bool out = false;
	int level = 1;
	bool isLevelUp = false; // level up?
	bool isLoadedSuperTank = false; // đã load super tank?
	bool isSuperTankAppear = false; // super tank xuất hiện?
	bool isAllowRenderSuperTank = false; // cho phép render super tank?
	bool isAllowRenderEffectSuperTank = false; // cho phép render hiện ứng xuất hiện super tank?
	bool isAllowSuperTankMove = false;  // cho phép super tank di chuyển?
	bool isAllowHandleAppearSuperTank = false; // cho phép xử lý sau khi tường nổ?
	bool isAllowTankMainMove = true; // cho phép tank main di chuyển?
	bool isAllowCreateTankBossList = false; // cho phép tạo bost list?
	bool isYouWin = false;

	MapGame map;
	TankBossList bossList;
	ItemList itemList;
	SuperTankBoss* superTankBoss = NULL ;

	map.loadMap("./image/mapimg5.png", "./general/mapgame.map", renderer);

	loadPauseMenu();
	if (Mix_PlayingMusic() == 0)
		Mix_PlayMusic(musicBackgroundGame, -1);
	if (load()) {

		tank.assign(renderer);

		SDL_Rect camera = { 0, 0, cameraWidth, cameraHeight }; // khai báo camera
		SDL_ShowCursor(SDL_DISABLE); // ẩn con trỏ chuột
		bossList.createListBoss(map, tank.getTankCircle(), level * 5, level < 5 ? level : 4, renderer, level * 50, level * 2, { 0, 0, 0 }, { 0, 0, 0 });
		
		while (!out) {
			while (SDL_PollEvent(&event) != 0) { // bắt các sự kiện
				if (event.type == SDL_QUIT) {
					out = true;
				}
				if (isAllowTankMainMove)
					tank.handleEvents(&event, camera);

				if (event.type == SDL_KEYUP) {
					if (event.key.keysym.sym == SDLK_ESCAPE) {
						if (handleMenuPause() == 1) { // về home
							Mix_HaltMusic();
							return;
						}
					}
				}

			}

			SDL_RenderClear(renderer); // clear màn hình render
			SDL_SetRenderDrawColor(renderer, 100, 50, 0, 0);

			if (!isYouWin) {
				if (isLevelUp) { // tăng độ level game
					level++;
					isLevelUp = false;
					std::cout << "level up" << std::endl;
					isAllowCreateTankBossList = true;
				}
				map.render(renderer, camera);
				if (level == TOTAL_LEVEL_GAME && !isLoadedSuperTank) {
					SuperTankBoss* _superTank = new SuperTankBoss;

					_superTank->loadImg(renderer);
					_superTank->loadEffectAppear(renderer);
					_superTank->loadDestroyImg(renderer);
					_superTank->handleEffectAppear(map.getMapWallDigital());
					isLoadedSuperTank = true;
					isAllowRenderEffectSuperTank = true;
					isAllowCreateTankBossList = false;
					isSuperTankAppear = true;
					superTankBoss = _superTank;
				}
				if (isAllowRenderEffectSuperTank) {
					if (superTankBoss->renderEffectAppear(renderer, camera)) {
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
					superTankBoss->setXY(backgroundWidth / 2 - superTankBoss->getW() / 2, backgroundHeight - 2 * cameraHeight);
					superTankBoss->setTankCircle(superTankBoss->getX(), superTankBoss->getY());
					superTankBoss->handleMoveAppear();
					isAllowHandleAppearSuperTank = false;
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
					if (superTankBoss->render(renderer, camera)) {
						if (isAllowSuperTankMove) {
							superTankBoss->handleSwitchLevel(tank.getTankCircle());
							if (superTankBoss->getIsDsestroy()) {
								bossList.clearList();
							}
							superTankBoss->move();
							superTankBoss->createBullet(renderer);
							tank.setDamgeReceived(superTankBoss->handleBullet(map, renderer, camera, tank.getTankCircle()));
							superTankBoss->renderBullet(renderer, camera);
						}
					}
					else {
						std::cout << "YOU WIN" << std::endl;
						delete superTankBoss;
						isYouWin = true;
					}
				}

				if (isAllowCreateTankBossList) {
					if (isAllowRenderSuperTank) {
						bossList.createListBoss(map, tank.getTankCircle(), level * 2, level < 5 ? level : 4, renderer, level * 50, level * 2, superTankBoss->getTankCircle(), superTankBoss->getCircleBallFire());
					}
					else {
						bossList.createListBoss(map, tank.getTankCircle(), level * 5, level < 5 ? level : 4, renderer, level * 50, level * 2, { 0, 0, 0 }, { 0, 0, 0 });
					}
					isAllowCreateTankBossList = false;
				}

				if (level >= 1 && SDL_GetTicks() - itemList.getSaveTimeCreate() >= TIME_RECREATE_ITEM) {
					itemList.createList(renderer, map);
				}

				if (isAllowTankMainMove) {
					if (isAllowRenderSuperTank) {
						tank.move(map, bossList, superTankBoss->getTankCircle(), superTankBoss->getCircleBallFire());
						tank.setChangeMoney(bossList.handleList(map, tank.getTankCircle(), renderer, camera, itemList, superTankBoss->getCircleBallFire()), smallFont, renderer);
						if (superTankBoss->getSwitchLevel()) {
							isLevelUp = true;
							superTankBoss->setSwitchLevel(false);
							std::cout << level << std::endl;
						}
					}
					else {
						tank.move(map, bossList, { 0, 0, 0 }, { 0, 0, 0 });
						tank.setChangeMoney(bossList.handleList(map, tank.getTankCircle(), renderer, camera, itemList, { 0, 0, 0 }), smallFont, renderer);
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
				tank.createBullet(renderer, musicChunk);
				bool _isSlowedTankMain = false;

				if (isAllowRenderSuperTank) { // có super tank
					tank.setDamgeReceived(bossList.handleBulletOfTankList(map, renderer, camera, tank.getTankCircle(), _isSlowedTankMain, superTankBoss->getCircleBallFire()));
					tank.handleBullet(map, renderer, camera, bossList, superTankBoss, musicChunk);
					superTankBoss->handleDamgeReceived(renderer, font);
				}
				else { // không có super tank
					tank.setDamgeReceived(bossList.handleBulletOfTankList(map, renderer, camera, tank.getTankCircle(), _isSlowedTankMain, { 0, 0, 0 }));
					tank.handleBullet(map, renderer, camera, bossList, NULL, musicChunk);
				}

				if (_isSlowedTankMain) {
					tank.setIsSlowed(true);
					tank.setSaveTimeIsSlowed(SDL_GetTicks());
				}

				itemList.renderItemlist(renderer, camera);
				tank.renderBullet(renderer, camera);
				bossList.renderBulletOfTankList(renderer, camera);
				tank.render(renderer, camera, font);
				bossList.renderList(renderer, camera, smallFont);
				tank.renderTam(renderer);
				if (tank.getIsDestroy()) {
					handleGameOverMenu();
					return;
				}
			}
			else {
				handleWinMenu();
				return;
			}

			SDL_RenderPresent(renderer);
		}
	}
	close();
	Mix_HaltMusic();
	SDL_ShowCursor(SDL_ENABLE); // hiện con trỏ chuột
	return;
}

int main(int arc, char* arg[]) {
	if (init()) {
		int selection = 0;
		loadMenuMain();
		loadAboutMenu();
		loadHomeMenu();
		while (true) {
			selection = handleMenuGame();
			if (selection == 0) { // chơi tiếp
				loadDataTank(false);
				loadGame();
				
				handleMenuHome();
			}
			else if (selection == 1) { // chơi mới
				loadDataTank(true);
				loadGame();

				handleMenuHome();
			}
			else if(selection == 2) { // about
				handleMenuAbout();
			}
			else if (selection == 3) { // exit
				break;
			}
		}
	}
	return 0;
}