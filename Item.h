#pragma once

#ifndef ITEM_H_
#define ITEM_H_

#include "BasicObject.h"
#include "MapGame.h"

#define TOTAL_ITEM 10
#define TOTAL_ITEM_TYPE 3

#define TIME_EXISTS_FIREBULLET_ITEM 8
#define TIME_EXISTS_HEALTH_ITEM 10
#define TIME_EXISTS_MONEY_ITEM 15

#define TIME_RECREATE_ITEM 20000

#define PLUS_HEALTH_ITEM 10
#define PLUS_FIREBULLET_ITEM 5
#define PLUS_MONEY_ITEM 10

class Item : public BasicObj {
public:
	enum ItemType {
		fireBulletItem, healthItem, moneyItem,
	};
private:
	ItemType type;
	Uint8 timeExists;
	bool isEat;
public:
	Item();
	~Item();
	void setType(ItemType _type) { type = _type; };
	ItemType getType() { return type; };
	void setTimeExists(Uint8 _time) { timeExists = _time; };
	Uint8 getTimeExists() { return timeExists; };
	void render(SDL_Renderer* _renderer, SDL_Rect _camera);
	void setIsEat(bool _is) { isEat = _is; };
	bool getIsEat() { return isEat; };
};

#endif // !ITEM_H_

#ifndef ITEMLIST_H_
#define ITEMLIST_H_

class ItemList {
private:
	std::vector<Item*> itemList;
	Uint32 saveTimeCreate;
public:
	ItemList() { saveTimeCreate = 0; };
	~ItemList() { ; };
	void createList(SDL_Renderer* _renderer, MapGame _map);
	void handleList();
	void renderItemlist(SDL_Renderer* _renderer, SDL_Rect _camera);
	Uint32 getSaveTimeCreate() { return saveTimeCreate; }
	std::vector<Item*> getItemList() { return itemList; };
};

#endif // !ITEMLIST_H_
