// Implementation for Texture class

#include "utils/texture.hpp"

Texture::Texture() {}

/// image texture constructor
void Texture::loadTexture(std::string path, SDL_Renderer* renderer) {
    SDL_Surface * surface = IMG_Load(path.c_str());
    if(!surface) {
        throw TextureLoadException(IMG_GetError());
    }

    // Create SDL texture from the surface
    auto newTexture = std::shared_ptr<SDL_Texture> 
        (SDL_CreateTextureFromSurface(renderer, surface), SDL_DestroyTexture);
    texture = newTexture;

    width = surface->w;
    height = surface->h;

    SDL_FreeSurface(surface);
}

void Texture::loadTextTexture(std::string textureText, SDL_Color textColor, 
    TTF_Font * font, SDL_Renderer* renderer) {

    SDL_Surface* surface = TTF_RenderText_Solid(font, textureText.c_str(), 
        textColor);
     if(surface == NULL) {
        throw TextureLoadException(TTF_GetError());
    }

    auto newTexture = std::shared_ptr<SDL_Texture> 
        (SDL_CreateTextureFromSurface(renderer, surface), SDL_DestroyTexture);
    texture = newTexture;

    width = surface->w;
    height = surface->h;
    SDL_FreeSurface(surface);
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
    double angle, SDL_Point * center, SDL_RendererFlip flip) const {
    SDL_Rect renderArea = {x, y, width, height};

    if(clip != NULL) {
        renderArea.w = clip->w;
        renderArea.h = clip->h;
    }

    SDL_RenderCopyEx(renderer, texture.get(), clip, &renderArea, angle, center, flip);
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
    SDL_SetTextureColorMod(texture.get(), red, green, blue);
}


/// Texture Blending
void Texture::setBlendMode(SDL_BlendMode blending) {
    SDL_SetTextureBlendMode(texture.get(), blending);
}

/// Transparency
void Texture::updateAlpha() {
    if(alphaIncreasing) {
        setAlpha(textureAlpha + ALPHA_STEP);

        if(textureAlpha == ALPHA_MAX) {
            alphaIncreasing = false;
        }
    } else {
        setAlpha(textureAlpha - ALPHA_STEP);

        if(textureAlpha == 0) {
            alphaIncreasing = true;
        }
    }
}

void Texture::setAlpha(Uint8 alpha) {
    SDL_SetTextureAlphaMod(texture.get(), alpha);
    textureAlpha = alpha;
}

int Texture::getHeight() {
    return height;
}

int Texture::getWidth() {
    return width;
}

std::shared_ptr<SDL_Texture> Texture::getTexture() {
    return texture;
}