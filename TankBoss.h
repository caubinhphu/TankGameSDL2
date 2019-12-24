#pragma once
#ifndef TANKBOSS_H_
#define TANKBOSS_H_

#include "BasicObject.h"
#include "MapGame.h"
#include "Bullet.h"

#define TOTAL_TANKBOSS_TYPE 4
#define TOTAL_DESTROY_IMG 4
#define RANGE_SQUARE 250000

class TankBoss : public BasicObj {
public:
	enum TankType {
		nomalTank, fireTank, iceTank, triangleTank,
	};
	enum Direction {
		TOP, BOTTOM, LEFT, RIGHT
	};
private:
	int spX, spY;
	double rotation;
	TankType type;
	Direction dir;
	std::vector<Bullet*> bullets;
	Bullet::BulletType bulletType;
	Bullet::DameBullet bulletDamge;
	Bullet::BulletFirtingRate bulletRate;
	std::string pathBulletImg;
	Circle tankCircle;
	bool isDestroy;
	int speed;
	BloodBar bloodBar;
	BasicObj destroyImg[TOTAL_DESTROY_IMG];
	int frameDestroy;
	Uint32 saveTimeShoot;
	int armor;
	int totalHealth;
	int healthCurrent;

	//BASIC_OBJECT text_blood_wasted;
	bool isDamage;
	int totalDamageWasted;
	// BASIC_OBJECT img_effect_money;
	int randomSpeed();
public:
	TankBoss();
	~TankBoss();
	void setTankCircle(int _x, int _y) { tankCircle = { _x + box.w / 2, _y + box.h / 2, box.w / 2 }; };
	Circle getTankCircle() { return tankCircle; };
	void setDirection(Direction _dir) { dir = _dir; };
	Direction getDirection() { return dir; };
	void setType(TankType _type) { type = _type; };
	TankType getType() { return type; };
	void setIsDestroy(bool _is) { isDestroy = _is; };
	bool getIsDestroy() { return isDestroy; };
	void render(SDL_Renderer* _renderer, SDL_Rect _camera);
	int getSpeed() { return speed; };

	void setPercentBlood(int _subPercent) { bloodBar.percent -= _subPercent; }
	void handleDirection();
	void handleMove(MapGame _map, Circle _tankMain, bool isCollisionTeams);
	double getRotation() { return rotation; }
	void randomDirection(int k);

	void setMinusHealthCurrent(int _minus) { healthCurrent -= _minus; };
	int getHealteCurrent() { return healthCurrent; };

	void loadDestroyImg(SDL_Renderer* _renderer);
	bool renderDestroy(SDL_Renderer* _renderer, SDL_Rect _camera);

	//std::vector<AMMO*> get_list_ammo() const { return ds_ammo; };
	//void set_list_ammo(std::vector<AMMO*> list_ammo) { ds_ammo = list_ammo; }

	void createBullet(SDL_Renderer* _renderer, Circle _tankMain);
	void handleBullet(MapGame _map, SDL_Renderer* _renderer, SDL_Rect _camera);
	void renderBullet(SDL_Renderer* _renderer, SDL_Rect _camera);

	void setArmor(int _armor) { armor = _armor; }
	int getArmor() { return armor; }

	void setTotalHealth(int _health) { totalHealth = _health; }
	int getTotalHealth() const { return totalHealth; }

	//void set_sound_destroy(const bool flag) {sound_destroy = flag;}
	//bool get_sound_destroy() const {return  sound_destroy;}

	//void load_text_blood_wasted(SDL_Renderer* RenDer, TTF_Font* font);
	//void render_text_blood_wasted(SDL_Renderer* RenDer, SDL_Rect cam);

	void setTotalDamageWasted(int _damge) { totalDamageWasted = _damge; }
	void setIsDamage(bool _flag) { isDamage = _flag; }

	//void load_img_effect_money(SDL_Renderer* RenDer);

	void setBulletDamge(Bullet::DameBullet _damge) { bulletDamge = _damge; };
	void setBulletType(Bullet::BulletType _type) { bulletType = _type; };
	void setBulletRate(Bullet::BulletFirtingRate _rate) { bulletRate = _rate; };
	void setPathBullet(std::string _path) { pathBulletImg = _path; };
};

#endif // !TANKBOSS_H_

#ifndef TANKBOSSES_H_
#define TANKBOSSES_H_

class TankBossList {
private:
	std::vector<TankBoss*> bossList;
public:
	TankBossList();
	~TankBossList();
	void createListBoss(MapGame _map, Circle _tankMain, int _quality, int _typeNum, SDL_Renderer* _renderer);
	bool checkCollisionTankBossList(Circle _boss, int k);
	void renderList(SDL_Renderer* _renderer, SDL_Rect _camera);
	void handleList(MapGame _map, Circle _tankMain, SDL_Renderer* _renderer, SDL_Rect _camera);
	bool checkCollisionBullet(SDL_Rect _bullet, bool _iSenemies, int _damgeBullet);
	void handleBulletOfTankList(MapGame _map, SDL_Renderer* _renderer, SDL_Rect _camera);
	void renderBulletOfTankList(SDL_Renderer* _renderer, SDL_Rect _camera);
};

#endif // !TANKBOSSES_H_

