#pragma once
#include "base.h"
#include "common.h"

class Text : public base 
{
public:
    Text();
    ~Text();

    enum TEXT_COLOR 
    {
        WHITE = 5
    };

    void SetText(const string& text) { text_ = text; }
    void SetColor(const int& type);
    void loadText_showText(TTF_Font* font, SDL_Renderer* screen);
private:
    string text_;
    SDL_Color text_color_;
};

