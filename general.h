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

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Event event;
static TTF_Font* font = NULL;
static TTF_Font* smallFont = NULL;
static TTF_Font* bigFont = NULL;

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
}

typedef struct BloodBar
{
	int x, y, width, height;
	int percent;
};

#endif // !GENERAL_H_