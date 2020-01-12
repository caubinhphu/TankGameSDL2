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

#ifdef _SDL_TTF_H
bool BasicObj::loadText(TTF_Font* _font, std::string _text, SDL_Color _color, SDL_Renderer* _renderer)
{
	SDL_Surface* surface = TTF_RenderText_Solid(_font, _text.c_str(), _color);
	if (surface == NULL) return false;
	box.w = surface->w;
	box.h = surface->h;
	texture = SDL_CreateTextureFromSurface(_renderer, surface);
	if (texture == NULL)
		return false;
	SDL_FreeSurface(surface);
	return true;
}
#endif

void BasicObj::render(SDL_Renderer* _renderer, int _x, int _y, SDL_Rect* _clip = NULL, double _aRadian = 0) {
	SDL_Rect rect = { _x, _y, box.w, box.h };
	if (_clip != NULL) {
		rect.w = _clip->w;
		rect.h = _clip->h;
	}

	SDL_RenderCopyEx(_renderer, texture, _clip, &rect, (_aRadian * 180) / PI, NULL, SDL_FLIP_NONE);
}

void BasicObj::setColor(Uint8 _r, Uint8 _g, Uint8 _b) {
	SDL_SetTextureColorMod(texture, _r, _g, _b);
}
