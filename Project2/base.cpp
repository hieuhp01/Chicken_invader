#include "base.h"

// texture manager
base::base() 
{
    object_ = NULL;
    rect_.x = 0;
    rect_.y = 0;
    rect_.w = 0;
    rect_.h = 0;
}
base::~base()
{
    Clean();
}
bool base::loadImg(string path, SDL_Renderer* screen) 
{
    Clean();
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadSurface = IMG_Load(path.c_str());
    if (loadSurface != NULL)
    {
        //Color key the image before creating texture from it
        SDL_SetColorKey(loadSurface, SDL_TRUE, SDL_MapRGB(loadSurface->format, 170, 170, 170));
        newTexture = SDL_CreateTextureFromSurface(screen, loadSurface);
        if (newTexture != NULL) 
        {
            rect_.w = loadSurface->w;
            rect_.h = loadSurface->h;
        }
        SDL_FreeSurface(loadSurface);
    }
    object_ = newTexture;
    return object_ != NULL;
}
void base::Render(SDL_Renderer* screen, const SDL_Rect* clip)
{
    SDL_Rect renderQuad = { rect_.x ,rect_.y , rect_.w ,rect_.h };
    SDL_RenderCopy(screen, object_, clip, &renderQuad);
}
void base::Clean()
{
    if (object_ != NULL) 
    {
        SDL_DestroyTexture(object_);
        object_ = NULL;
        rect_.w = 0;
        rect_.h = 0;
    }
}
