#pragma once

#ifndef GENERAL_H_
#define GENERAL_H_

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#define backgroundWidth 1600
#define backgroundHeight 1600
#define cameraWidth 650
#define cameraHeight 450
#define PI 3.14159265359 // số PI

#define SOUND_SHOOT_FIRE 0
#define SOUND_SHOOT_NORMAL 1
#define SOUND_COLLISION_WALL 2
#define SOUND_COLLISION_ENEMIES 3
#define SOUND_NULL_BULLET 4
#define SOUND_EXPLOSION 5
#define SOUND_MOUSE_INSIDE 6
#define SOUND_SHOOT_ROCKET 7


static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Event event;
static TTF_Font* font = NULL;
static TTF_Font* smallFont = NULL;
static TTF_Font* bigFont = NULL;

static Mix_Music* musicBackgroundGame = NULL;
static Mix_Music* musicMenuMain = NULL;
static Mix_Music* musicMenuHome = NULL;
static Mix_Music* musicMenuShop = NULL;
static Mix_Chunk* musicChunk[8] = { NULL };

struct Circle
{
	int x, y, r;
};

namespace check {
	double rotationA_B(int xA, int yA, int xB, int yB); // return radian
	bool checkRect_Rect(SDL_Rect a, SDL_Rect b);
	int binhPhuongKhoangCach(int x1, int y1, int x2, int y2);
	bool checkRect_Circle(SDL_Rect b, Circle a);
	bool checkCircle_Circle(Circle a, Circle b);
	bool checkInsideRect(int _x, int _y, SDL_Rect a);
}

typedef struct BloodBar
{
	int x, y, width, height;
	int percent;
};

#endif // !GENERAL_H_