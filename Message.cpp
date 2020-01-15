#include "Message.h"

Message::Message(std::string _textNameMessage, std::string _textMessage, std::vector<std::string> _textOption) {
	textNameMessage = _textNameMessage;
	textMessage = _textMessage;
	textOption = _textOption;
}

void Message::createMessage(SDL_Renderer* _renderer, TTF_Font* _font) {
	BasicObj::loadImg("./image/message.png", _renderer);
	textNameMessageImg.loadText(_font, textNameMessage, { 0, 0, 0 }, _renderer);
	textMessageImg.loadText(_font, textMessage, { 0, 0, 0 }, _renderer);
	textOptionImg.resize(textOption.size());
	int _x = 510;
	for (int i = 0; i < textOption.size(); i++) {
		textOptionImg[i].loadText(_font, textOption[i], { 255, 255, 255 }, _renderer);
		textOptionImg[i].setXY(_x - textOptionImg[i].getW(), 260);
		_x -= textOptionImg[i].getW() + 10;
	}
}

void Message::renderMessage(SDL_Renderer* _renderer) {
	BasicObj::render(_renderer, 120, 100, NULL, 0);
	textNameMessageImg.render(_renderer, 130, 110, NULL, 0);
	textMessageImg.render(_renderer, 130, 170, NULL, 0);
	int _x = 510;
	for (int i = 0; i < textOptionImg.size(); i++) {
		textOptionImg[i].render(_renderer, textOptionImg[i].getX(), 260, NULL, 0);
		_x -= textOptionImg[i].getW() + 10;
	}
}

std::string Message::handleMessage(SDL_Renderer* _renderer) {
	bool out = false;
	
	while (!out) {
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT)
				out = true;
			else if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
				int _x = 0, _y = 0;
				SDL_GetMouseState(&_x, &_y); // lấy tọa độ chuột
				for (int i = 0; i < textOptionImg.size(); i++) {
					if (check::checkInsideRect(_x, _y, textOptionImg[i].getBox())) {
						textOptionImg[i].setColor(0, 0, 128);
						if (e.type == SDL_MOUSEBUTTONDOWN) {
							return textOption[i];
						}
					}
					else {
						textOptionImg[i].setColor(255, 255, 255);
					}
				}
			}
		}
		renderMessage(_renderer);
		SDL_RenderPresent(_renderer);
	}
	return "OUT";
}
