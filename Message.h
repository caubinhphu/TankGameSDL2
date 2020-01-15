#pragma once

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "BasicObject.h"

class Message : public BasicObj {
private:
	std::string textNameMessage;
	BasicObj textNameMessageImg;
	std::string textMessage;
	BasicObj textMessageImg;
	std::vector<std::string> textOption;
	std::vector<BasicObj> textOptionImg;
public:
	Message(std::string _textNameMessage, std::string textMessage, std::vector<std::string> _textOption);
	~Message() { ; };
	void createMessage(SDL_Renderer* _renderer, TTF_Font* _font);
	void renderMessage(SDL_Renderer* _renderer);
	std::string handleMessage(SDL_Renderer* _renderer);
};

#endif // !MESSAGE_H_
