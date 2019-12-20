#include "TankMain.h"

TankMain::TankMain(int _x, int _y) {
	box.x = _x;
	box.y = _y;
	speed = 5;
	spX = spY = 0;
	rotation = 0;
	//setTankCircle();
}

TankMain::~TankMain() {
	free();
}

void TankMain::setTankCircle() {
	tankCircle = { box.x + box.w / 2, box.y + box.h / 2, box.w / 2 };
}

void TankMain::handleEvents(SDL_Event* _e, SDL_Rect _camera) {
	int mouseX = 0, mouseY = 0;
	SDL_GetMouseState(&mouseX, &mouseY);

	rotation = check::rotationA_B(mouseX, mouseY, box.x + box.w / 2 - _camera.x, box.y + box.h / 2 - _camera.y);
	//std::cout << rotation << std::endl;

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

void TankMain::move(MapGame map) {
	box.x += spX;
	box.y += spY;
	setTankCircle();
	if (map.checkCollision(tankCircle)) {
		box.x -= spX;
		setTankCircle();
	}
	if (map.checkCollision(tankCircle)) {
		box.y -= spY;
		setTankCircle();
	}
	
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
	BasicObj::render(_renderer, box.x - _camera.x, box.y - _camera.y, NULL, rotation);
}

bool TankMain::loadTamBan(std::string _path, SDL_Renderer* _renderer) {
	return tamBan.loadImg(_path, _renderer);
}

void TankMain::renderTam(SDL_Renderer* _renderer) {
	int mouseX = 0, mouseY = 0;
	SDL_GetMouseState(&mouseX, &mouseY);
	tamBan.render(_renderer, mouseX - tamBan.getW() / 2, mouseY - tamBan.getH() / 2, NULL, 0);
}