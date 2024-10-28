#pragma once
#include <SDL.h>
#include <string>

class TextureManager 
{
public:
    static SDL_Texture* loadTexture(const std::string& fileName, SDL_Renderer* renderer);
    static void render(SDL_Texture* tex, SDL_Renderer* ren, SDL_Rect dst);
    static void clean(SDL_Texture* tex);

};
