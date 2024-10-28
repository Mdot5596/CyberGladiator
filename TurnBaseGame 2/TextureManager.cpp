#include "TextureManager.h"
#include <SDL_image.h>
#include <iostream>



SDL_Texture* TextureManager::loadTexture(const std::string& fileName, SDL_Renderer* renderer) 
{
    SDL_Texture* texture = IMG_LoadTexture(renderer, fileName.c_str());
    if (!texture) 
    {
        std::cout << "Failed to load texture: " << fileName << " - " << IMG_GetError() << std::endl;
    }
    return texture;
}



void TextureManager::render(SDL_Texture* tex, SDL_Renderer* ren, SDL_Rect dst) 
{
    SDL_RenderCopy(ren, tex, nullptr, &dst);
}


void TextureManager::clean(SDL_Texture* text)
{
    SDL_DestroyTexture(text);
}