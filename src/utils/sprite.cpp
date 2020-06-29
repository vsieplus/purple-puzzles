#include "utils/sprite.hpp"

Sprite::Sprite(std::shared_ptr<Texture> spritesheet, const SDL_Rect & clip) : 
    spritesheet(spritesheet), spriteClip(clip) {}

// render the sprite at the specified area
void Sprite::render(SDL_Renderer * renderer, const SDL_Rect & renderArea,
    double angle, SDL_Point * center, SDL_RendererFlip flip) const {
    SDL_RenderCopyEx(renderer, spritesheet->getTexture().get(), &spriteClip, 
        &renderArea, angle, center, flip);
}

int Sprite::getWidth() const {
    return spriteClip.w;
}

int Sprite::getHeight() const {
    return spriteClip.h;
}