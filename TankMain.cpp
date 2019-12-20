#include "TankMain.h"

TankMain::TankMain(int _x, int _y) {
	box.x = _x;
	box.y = _y;
	speed = 5;
	spX = spY = 0;
}

TankMain::~TankMain() {
	free();
}

void TankMain::handleEvents(SDL_Event* _e, SDL_Rect _camera) {
	if (_e->type == SDL_KEYDOWN && _e->key.repeat == 0) {
		switch (_e->key.keysym.sym) {
		case SDLK_a:
			spX -= speed; break;
		case SDLK_d:
			spX += speed; break;
		case SDLK_s:
			spY += speed; break;
		case SDLK_w:
			spY -= speed; break;
		}
	}
	else if (_e->type == SDL_KEYUP && _e->key.repeat == 0) {
		switch (_e->key.keysym.sym) {
		case SDLK_a:
			spX += speed; break;
		case SDLK_d:
			spX -= speed; break;
		case SDLK_s:
			spY -= speed; break;
		case SDLK_w:
			spY += speed; break;
		}
	}
}

void TankMain::move() {
	box.x += spX;
	box.y += spY;
}

void TankMain::setCamera(SDL_Rect &_camera) {
	_camera.x = (box.x + box.w / 2) - cameraWidth / 2;
	_camera.y = (box.y + box.h / 2) - cameraHeight / 2;

	if (_camera.x < 0) {
		_camera.x = 0;
	}
	if (_camera.x > backgroundWidth - cameraWidth) {
		_camera.x = backgroundWidth - cameraWidth;
	}
	if (_camera.y < 0) {
		_camera.y = 0;
	}
	if (_camera.y > backgroundHeight - cameraHeight) {
		_camera.y = backgroundHeight - cameraHeight;
	}
}

void TankMain::render(SDL_Renderer* _renderer, SDL_Rect _camera) {
	BasicObj::render(_renderer, box.x - _camera.x, box.y - _camera.y, NULL, 0);
}