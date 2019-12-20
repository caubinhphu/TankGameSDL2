#pragma once

#ifndef GENERAL_H_
#define GENERAL_H_

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <SDL.h>
#include <SDL_image.h>

#define backgroundWidth 1600
#define backgroundHeight 1600
#define cameraWidth 650
#define cameraHeight 450

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Event event;

struct Circle
{
	int x, y, r;
};

namespace check {
	double rotationA_B(int xA, int yA, int xB, int yB);
	bool checkRect_Rect(SDL_Rect a, SDL_Rect b);
	int bingPhuongKhoangCach(int x1, int y1, int x2, int y2);
	bool checkRect_Circle(SDL_Rect b, Circle a);
}

#endif // !GENERAL_H_