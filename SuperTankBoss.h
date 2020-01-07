#pragma once

#ifndef SUPER_TANKBOSS_H_
#define SUPER_TANKBOSS_H_

#include "BasicObject.h"
#include "MapGame.h"
#include "Bullet.h"

#define TOTAL_IMG_EFFECT_APPEAR 5
#define TOTAL_IMG_REDZONE 5
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
	//AMMO::dame_bullet damebullet;
	Uint32 saveTimeShoot; // lưu thời gian bắn viên đạn trước
	// Uint32 saveTimeRedzone; // lưu thời gian xuất hiện redzone trước
	Uint32 timeBulletLv1, timeBulletLv2; // 
	// std::vector<BasicObj*> list_red_zone;
	// BASIC_OBJECT explosion_redzone[total_img_red_zone];
	// int frame_ex_rz;
	// bool is_create_red_zone;
	// bool allow_create_rz;
	// bool is_render_circle_rz;
	BasicObj textMinusHealth;
	//bool is_damage;
	//int total_damage_wasted;
	//bool is_create_boss;
	//bool allow_create_boss;
	//bool is_render_shield;
	//BASIC_OBJECT img_shield;
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
	void render(SDL_Renderer* _renderer, SDL_Rect _camera);

	void setDamgeReceived(int _damgeReceived) { damgeReceived += _damgeReceived; };
	void handleDamgeReceived(SDL_Renderer* _renderer, TTF_Font* _font);

	//void set_armor(const int ar) { armor = ar; }
	//int get_armor() const { return armor; }

	//void set_total_health(const int h) { total_health = h; }
	//int get_total_health() const { return total_health; }

	//void render_blood_bar(SDL_Renderer* RenDer, SDL_Rect cam, TTF_Font* _font);

	//bool check_collision_bullet(SDL_Rect bullet, int dame_bullet, bool is_enemies);

	void freePositionWallTileList() { positionWallTileList.clear(); }

	void createBullet(SDL_Renderer* _renderer);
	int handleBullet(MapGame _map, SDL_Renderer* _renderer, SDL_Rect _camera, Circle _tankMain);
	void renderBullet(SDL_Renderer* _renderer, SDL_Rect _camera);
	void createMarblesBullet(int _marblesLv, SDL_Renderer* _renderer, SDL_Rect _parentBullet, double _rotationParentBullet);

	//void load_ball_fire(SDL_Renderer* RenDer, std::string path);
	//void render_ball_fire(SDL_Renderer* RenDer, SDL_Rect cam);

	void setCircleBallFire(int _x, int _y);
	Circle getCircleBallFire() { return ballFireCircle; };

	//std::vector<AMMO*> get_list_bullet() const { return ds_bullet; };
	//void set_list_bullet(std::vector<AMMO*> list_bl) { ds_bullet = list_bl; }
	//void handle_bullet(SDL_Renderer* RenDer, SDL_Rect box_main);
	//void render_bullet(SDL_Renderer* RenDer, SDL_Rect cam, MAP_GAME map, circle char_main);
	//void set_type_bullet(const AMMO::type_ammo type) { type_bullet = type; }

	//void load_img_redzone(SDL_Renderer* RenDer);
	//void create_redzone(int quantity, SDL_Renderer* RenDer);
	//void render_redzone(SDL_Renderer* RenDer, SDL_Rect cam);
	//bool render_explosion_redzone(SDL_Renderer* RenDer, SDL_Rect cam);

	//void set_is_create_redzone(const bool flag) { is_create_red_zone = flag; }
	//bool get_is_create_redzone() const { return is_create_red_zone; }

	//void set_allow_create_redzone(const bool flag) { allow_create_rz = flag; }
	//bool get_allow_create_redzone() const { return allow_create_rz; }

	//void set_is_render_circle_rz(const bool flag) { is_render_circle_rz = flag; }
	//bool get_is_render_circle_rz() const { return is_render_circle_rz; }

	//void set_frame_rz(const int fr) { frame_ex_rz = fr; }
	//int get_frame_rz() const { return frame_ex_rz; }

	//std::vector<circle> get_circle_list_redzone();
	//circle create_circle(SDL_Rect _box);

	//void load_text_blood_wasted(SDL_Renderer* RenDer, TTF_Font* _font);
	//void render_text_blood_wasted(SDL_Renderer* RenDer, SDL_Rect cam);

	//void set_time_lv1(const Uint32 _time) { time_lv1 = _time; }
	//Uint32 get_time_lv1() const { return time_lv1; }

	//void set_time_lv2(const Uint32 _time) { time_lv2 = _time; }
	//Uint32 get_time_lv2() const { return time_lv2; }

	//void handle_marbles_bullet(int type_marbles, SDL_Renderer* RenDer, SDL_Rect box_bullet, double rotation_bullet);

	//void set_is_create_boss(const bool flag) { is_create_boss = flag; }
	//bool get_is_create_boss() const { return is_create_boss; }

	//void set_allow_create_boss(const bool flag) { allow_create_boss = flag; }
	//bool get_allow_create_boss() const { return allow_create_boss; }

	//void load_img_shield(SDL_Renderer* RenDer, std::string path);
	//void render_img_shield(SDL_Renderer* RenDer, SDL_Rect cam);

	//void set_is_render_shields(const bool flag) { is_render_shield = flag; }
	//bool get_is_render_shields() const { return is_render_shield; }

	//void load_ds_bullet_reuse(SDL_Renderer* RenDer);

	//bool get_is_destroy() const { return is_destroy; }
	//void set_is_destroy(const bool flag) { is_destroy = flag; }

};

#endif // !SUPER_TANKBOSS_H_