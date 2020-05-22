// Implementation for Texture class

#include "utils/texture.hpp"

Texture::Texture() {}

/// image texture constructor
void Texture::loadTexture(std::string path, SDL_Renderer* renderer) {
    SDL_Texture* newTexture = NULL;

    SDL_Surface* surface = IMG_Load(path.c_str());
    if(surface == NULL) {
        throw TextureLoadException(IMG_GetError());
    }

    // Create SDL texture from the surface
    newTexture = SDL_CreateTextureFromSurface(renderer, surface);
    width = surface->w;
    height = surface->h;

    SDL_FreeSurface(surface);

    texture = newTexture;
}

void Texture::loadTextTexture(std::string textureText, SDL_Color textColor, TTF_Font * font,
    SDL_Renderer* renderer) {

    SDL_Surface* surface = TTF_RenderText_Solid(font, textureText.c_str(), 
        textColor);
     if(surface == NULL) {
        throw TextureLoadException(TTF_GetError());
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    width = surface->w;
    height = surface->h;
    SDL_FreeSurface(surface);
}

Texture::~Texture() {
    freeTexture();
}


void Texture::freeTexture() {
    if(texture != NULL) {
        SDL_DestroyTexture(texture);
        width = 0;
        height = 0;
        texture = NULL;
    }
}

/**
 * @brief Render the texture at position x,y to the given renderer
 * 
 * @param x the x coordinate to render at
 * @param y the y coord to render at
 * @param renderer the SDL_renderer to use
 * @param clip optional SDL_Rect defining which portion of the texture to render
 * @param angle opt. double specifying angle to render texture at
 * @param center center loc. of rendering
 * @param flip Type of flip to use when rendering
 */
void Texture::render(int x, int y, SDL_Renderer * renderer, SDL_Rect * clip,
    double angle, SDL_Point * center, SDL_RendererFlip flip) {
    SDL_Rect renderArea = {x, y, width, height};

    if(clip != NULL) {
        renderArea.w = clip->w;
        renderArea.h = clip->h;
    }

    SDL_RenderCopyEx(renderer, texture, clip, &renderArea, angle, center, flip);
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
    SDL_SetTextureColorMod(texture, red, green, blue);
}


/// Texture Blending
void Texture::setBlendMode(SDL_BlendMode blending) {
    SDL_SetTextureBlendMode(texture, blending);
}

/// Transparency
void Texture::setAlpha(Uint8 alpha) {
    SDL_SetTextureAlphaMod(texture, alpha);
}

int Texture::getHeight() {
    return height;
}

int Texture::getWidth() {
    return width;
}