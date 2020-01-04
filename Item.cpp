#include "Item.h"

// ------------------------------ Item ------------------------------------
Item::Item() {
	type = fireBulletItem;
	timeExists = 0;
	isEat = false;
	timeCreate = 0;
}

Item::~Item() {
	;
}

void Item::render(SDL_Renderer* _renderer, SDL_Rect _camera)
{
	BasicObj::render(_renderer, box.x - _camera.x, box.y - _camera.y, NULL, 0);
}








// ------------------------ Item list ---------------------------------------

void ItemList::createList(SDL_Renderer* _renderer, MapGame _map) {
	for (int i = 0; i < TOTAL_ITEM; i++) {
		Item* _item = new Item();
		int _type = 1 + rand() % TOTAL_ITEM_TYPE;
		if (_type == 1) {
			_item->loadImg("./image/bullet_item.png", _renderer);
			_item->setType(Item::fireBulletItem);
			_item->setTimeExists(TIME_EXISTS_FIREBULLET_ITEM);
		}
		else if (_type == 2) {
			_item->loadImg("./image/health_item.png", _renderer);
			_item->setType(Item::healthItem);
			_item->setTimeExists(TIME_EXISTS_HEALTH_ITEM);
		}
		else if (_type == 3) {
			_item->loadImg("./image/money_item.png", _renderer);
			_item->setType(Item::moneyItem);
			_item->setTimeExists(TIME_EXISTS_MONEY_ITEM);
		}

		// tìm vị trí phù hợp
		SDL_Rect _box = { 0, 0, _item->getW(), _item->getH() };
		do {
			_box.x = rand() % (backgroundWidth - _box.w);
			_box.y = rand() % (backgroundHeight - _box.h);
		} while (_map.checkCollisionRect(_box));
		_item->setXY(_box.x, _box.y);
		itemList.push_back(_item);
	}
	saveTimeCreate = SDL_GetTicks();
}

void ItemList::handleList() {
	for (int i = 0; i < itemList.size(); i++) {
		if (itemList[i]->getIsEat() || SDL_GetTicks() - saveTimeCreate > itemList[i]->getTimeExists() * 1000) {
			delete itemList[i];
			itemList.erase(itemList.begin() + i);
			i--;
		}
	}
	//if (itemListFromBossDestroy.size() != 0) std::cout << 1 << std::endl;
	//for (int i = 0; i < itemListFromBossDestroy.size(); i++) {
	//	if (itemListFromBossDestroy[i]->getIsEat()
	//			|| SDL_GetTicks() - itemListFromBossDestroy[i]->getTimeCreate() > itemListFromBossDestroy[i]->getTimeExists() * 1000) {
	//		delete itemListFromBossDestroy[i];
	//		itemListFromBossDestroy.erase(itemListFromBossDestroy.begin() + i);
	//		i--;
	//	}
	//}
}

void ItemList::renderItemlist(SDL_Renderer* _renderer, SDL_Rect _camera) {
	for (int i = 0; i < itemList.size(); i++) {
		itemList[i]->render(_renderer, _camera);
	}
	//for (int i = 0; i < itemListFromBossDestroy.size(); i++) {
	//	itemListFromBossDestroy[i]->render(_renderer, _camera);
	//}
}

//void ItemList::createItemListFromBossDestroy(SDL_Renderer* _renderer, Circle _tank) {
//	Item* _item = new Item();
//	int _type = 1 + rand() % TOTAL_ITEM_TYPE;
//	if (_type == 1) {
//		_item->loadImg("./image/bullet_item.png", _renderer);
//		_item->setType(Item::fireBulletItem);
//		_item->setTimeExists(TIME_EXISTS_FIREBULLET_ITEM);
//	}
//	else if (_type == 2) {
//		_item->loadImg("./image/health_item.png", _renderer);
//		_item->setType(Item::healthItem);
//		_item->setTimeExists(TIME_EXISTS_HEALTH_ITEM);
//	}
//	else if (_type == 3) {
//		_item->loadImg("./image/money_item.png", _renderer);
//		_item->setType(Item::moneyItem);
//		_item->setTimeExists(TIME_EXISTS_MONEY_ITEM);
//	}
//	_item->setXY(_tank.x - _item->getW() / 2, _tank.y - _item->getH() / 2);
//	_item->setTimeCreate(SDL_GetTicks());
//	itemListFromBossDestroy.push_back(_item);
//}


