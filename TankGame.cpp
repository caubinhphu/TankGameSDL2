#include "general.h"
#include "TankMain.h"

TankMain tank(100, 100);

bool init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		return false;
	}
	window = SDL_CreateWindow("TANK GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, cameraWidth, cameraHeight, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		return false;
	}
	renderder = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderder == NULL) {
		return false;
	}
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		return false;
	}

	return true;
}

bool load() {
	return tank.loadImg("./image/tank5.png", renderder);
}

void close() {
	tank.free();
	SDL_DestroyRenderer(renderder);
	renderder = NULL;
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
}

int main(int arc, char* arg[]) {

	if (init()) {
		bool out = false;
		if (load()) {
			SDL_Rect camera = { 0, 0, cameraWidth, cameraHeight };
			while (!out) {
				while (SDL_PollEvent(&event) != 0) {
					if (event.type == SDL_QUIT) {
						out = true;
					}
					tank.handleEvents(&event, camera);
				}

				SDL_RenderClear(renderder);
				SDL_SetRenderDrawColor(renderder, 100, 50, 0, 0);
				
				tank.move();
				tank.setCamera(camera);

				tank.render(renderder, camera);
				SDL_RenderPresent(renderder);
			}
			
		}
		close();
	}

	return 0;
}