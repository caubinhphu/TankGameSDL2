﻿#pragma once
#ifndef BASICOBJECT_H_
#define BASICOBJECT_H_

#include "general.h"

class BasicObj {
protected:
	SDL_Rect box;
	SDL_Texture* texture;
public:
	BasicObj(); // hàm khởi tạo
	~BasicObj(); // hàm hủy
	void free(); // hàm giải phóng bộ nhớ
	virtual bool loadImg(std::string _path, SDL_Renderer* _renderer); // hàm load hình ảnh
#ifdef _SDL_TTF_H
	bool loadText(TTF_Font* _font, std::string _text, SDL_Color _color, SDL_Renderer* _renderer);
#endif
	virtual void render(SDL_Renderer* _renderer, int _x, int _y, SDL_Rect* _clip, double _aRadian); // hàm render
	int getX() { return box.x; }; // hàm lấy toạ độ x của đối tượng
	int getY() { return box.y; }; // hàm lấy toạ độ y của đối tượng
	int getW() { return box.w; }; // hàm lấy width của đối tượng
	int getH() { return box.h; }; // hàm lấy heigth của đối tượng
	void setXY(int _x, int _y) { box.x = _x; box.y = _y; }; // hàm set toạ độ (x, y) cho đối tượng
	SDL_Rect getBox() { return box; };

	void setAlphaMod(Uint8 _alpha) { SDL_SetTextureAlphaMod(texture, _alpha); };
	void setColor(Uint8 _r, Uint8 _g, Uint8 _b);
};



#endif // !BASICOBJECT_H_

