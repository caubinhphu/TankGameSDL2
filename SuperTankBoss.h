#pragma once

#ifndef SUPER_TANKBOSS_H_
#define SUPER_TANKBOSS_H_

#include "BasicObject.h"
#include "MapGame.h"
#include "Bullet.h"

#define TOTAL_IMG_EFFECT_APPEAR 5
#define TOTAL_DESTROY_SUPERTANK_IMG 4

class SuperTankBoss : public BasicObj
{
private:
	int spX, spY, speed; // độ lêch x, y, tốc độ của super tank boss
	Circle tankCircle; // dạng hình tròn
	BloodBar bloodBar; // thanh máu
	int totalHealth; // tổng máu
	int healthCurrent; // máu hiện tại
	int armor; // giáp
	double rotation; // góc quay
	BasicObj effectAppear[TOTAL_IMG_EFFECT_APPEAR]; // mảng hình ảnh hiệu ứng xuất hiện
	int frame; // khung load hình ảnh xuất hiện (nổ tường) hiện tại
	std::vector<std::vector<int>> positionWallTileList;
	bool isDestroy; // bị phá hủy?
	BasicObj ballFire; // hình ảnh cầu lửa xung quanh tank
	Circle ballFireCircle; // danh hình tròn cầu lửa xung quanh tank

	std::vector<Bullet*> bullets; // ds đạn
	int damgeReceived;
	bool isMinusHealth;
	Bullet::BulletType bulletType; // loại đạn hiện tại
	Uint32 saveTimeShoot; // lưu thời gian bắn viên đạn trước
	Uint32 timeBulletLv1, timeBulletLv2; // 

	BasicObj textMinusHealth;
	BasicObj shield;
	bool isAllowRenderShield;
	bool isSwitchLevel;
	int switchLevel;

	BasicObj destroyImg[TOTAL_DESTROY_SUPERTANK_IMG];
	int frameDestroy;
public:
	SuperTankBoss();
	~SuperTankBoss();

	void setTankCircle(int _x, int _y);
	Circle getTankCircle() { return tankCircle; }

	void loadEffectAppear(SDL_Renderer* _renderer);
	void handleEffectAppear(std::vector<int> _mapWall);
	bool renderEffectAppear(SDL_Renderer* _renderer, SDL_Rect _camera);

	void handleMoveAppear();
	bool moveAuto(int _y);

	void handleMove(Circle _tankMain, int _y);
	void handleSwitchLevel(Circle _tankMain);
	void move();

	bool loadImg(SDL_Renderer* _renderer);
	bool render(SDL_Renderer* _renderer, SDL_Rect _camera);

	void setDamgeReceived(int _damgeReceived) { damgeReceived += _damgeReceived; };
	void handleDamgeReceived(SDL_Renderer* _renderer, TTF_Font* _font);

	void freePositionWallTileList() { positionWallTileList.clear(); }

	void createBullet(SDL_Renderer* _renderer);
	int handleBullet(MapGame _map, SDL_Renderer* _renderer, SDL_Rect _camera, Circle _tankMain);
	void renderBullet(SDL_Renderer* _renderer, SDL_Rect _camera);
	void createMarblesBullet(int _marblesLv, SDL_Renderer* _renderer, SDL_Rect _parentBullet, double _rotationParentBullet);

	void setCircleBallFire(int _x, int _y);
	Circle getCircleBallFire() { return ballFireCircle; };

	void setSwitchLevel(bool _is) { isSwitchLevel = _is; };
	bool getSwitchLevel() { return isSwitchLevel; };

	bool getIsDsestroy() { return isDestroy; }

	void loadDestroyImg(SDL_Renderer* _renderer);
	bool renderDestroy(SDL_Renderer* _renderer, SDL_Rect _camera);
};

#endif // !SUPER_TANKBOSS_H_