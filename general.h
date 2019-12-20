#pragma once

#ifndef GENERAL_H_
#define GENERAL_H_

#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

#define backgroundWidth 1600
#define backgroundHeight 1600
#define cameraWidth 650
#define cameraHeight 450

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Event event;

namespace check {
	double rotationA_B(int xA, int yA, int xB, int yB);
}

#endif // !GENERAL_H_