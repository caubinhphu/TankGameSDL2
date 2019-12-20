#include "BasicObject.h"

BasicObj::BasicObj() {
	box = { 0, 0, 0, 0 };
	texture = NULL;
}

BasicObj::~BasicObj() {
	free();
}

void BasicObj::free() {
	box = { 0, 0, 0, 0 };
	SDL_DestroyTexture(texture);
	texture = NULL;
}

bool BasicObj::loadImg(std::string _path, SDL_Renderer* _renderer) {
	SDL_Surface* surface = IMG_Load(_path.c_str());
	if (surface == NULL) {
		return false;
	}

	box.w = surface->w; // lấy width của ảnh
	box.h = surface->h; // lấy height của ảnh

	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 134, 134, 134)); // xóa màu nền: màu nền (r, g, b) = (134, 134, 134)
	texture = SDL_CreateTextureFromSurface(_renderer, surface);
	if (texture == NULL) {
		return false;
	}
	SDL_FreeSurface(surface);
	surface = NULL;
	return true;
}

void BasicObj::render(SDL_Renderer* _renderer, int _x, int _y, SDL_Rect* _clip = NULL, double _a = 0) {
	SDL_Rect rect = { _x, _y, box.w, box.h };
	if (_clip != NULL) {
		rect.w = _clip->w;
		rect.h = _clip->h;
	}
	SDL_RenderCopyEx(_renderer, texture, _clip, &rect, _a, NULL, SDL_FLIP_NONE);
}