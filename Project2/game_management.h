#pragma once
#include "Common.h"
#include "base.h"
#include "space_ship.h"
#include "chicken.h"
#include "Explosion.h"
#include "Text.h"

class game_management
{
public:
    game_management();
    ~game_management();

    void init(std::string title);
    void handle_event();
    void handle_game();
    void handle_chicken();
    void clean();
    bool get_running() { return isRunning; }
    bool check_collision(const SDL_Rect& object1, const SDL_Rect& object2);
private:
    bool isRunning;
    SDL_Window* gWindow;
    SDL_Renderer* gRenderer;
    SDL_Event gEvent;
    TTF_Font* g_font_text;
    TTF_Font* g_font_menu;
    TTF_Font* g_font_end_game;

    base background;
    base support;
    space_ship spaceship;
    Explosion exp;
    Text kill_text;
    Text heart_text;
    Text hint;
    Text end_game;

    int kill;
    float scrolling;
    vector<Chicken*> p_chicken_list;
};

