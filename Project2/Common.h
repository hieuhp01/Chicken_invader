#pragma once
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

using namespace std;

const int BACKGROUND_WIDTH = 1200;
const int BACKGROUND_HEIGHT = 3840;
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 760;
const float SCREEN_SPEED = 0.4;


const int RENDER_DRAW_COLOR = 255;

const int NUMBER_OF_CHICKEN = 36;
const int CHICKEN_HEART = 3;
const int DISTANCE_BETWEEN_CHICKENS = 100;
const int NUMBER_OF_CHICKENS_PER_ROW = 12;
const int BLASTER = 0;

struct StatusChicken {
    int turn_left;
    int turn_right;
    int turn_up;
    int turn_down;
};